import sys, os, xml, zipfile, string, tarfile, getopt, shutil, popen2
from os import path
from os.path import join
from xml.dom import minidom, Node
from zipfile import ZipFile
from tarfile import TarFile, TarFileCompat

windows  =  sys.platform == 'win32'
linux    =  sys.platform == 'linux2'
mac      =  sys.platform == 'darwin'
colladaVersion = '1.4'
domVersion = '2.0'
colladaVersionNoDots =  colladaVersion.replace('.', '')
domVersionNoDots = domVersion.replace('.', '')
sfdom = 'https://collada-dom.svn.sourceforge.net/svnroot/collada-dom'
archivePrefix = 'colladadom'

def disableDebugInfoRecursive(node):
    if node.nodeType == Node.ELEMENT_NODE and node.tagName == 'Tool':
        attr = node.getAttributeNode('DebugInformationFormat')
        if attr:
            attr.nodeValue = '0'
        attr = node.getAttributeNode('GenerateDebugInformation')
        if attr:
            attr.nodeValue = 'false'
    map(disableDebugInfoRecursive, node.childNodes)

def disableProjectDebugInfo(projectPath):
    root = minidom.parse(projectPath)
    disableDebugInfoRecursive(root.childNodes[0])
    root.writexml(open(projectPath, 'w'))

def disableDebugInfo(codePath):
    projs = [join(codePath, 'dom', 'projects', 'vc8', name + '.vcproj') \
                 for name in ['dom', 'dom-static', 'domTest']]
    for proj in projs:
        disableProjectDebugInfo(proj)

def setupReleaseCheckout(opts):
    outdir = getOutdir(opts)
    path0 = join(outdir, 'dom-vc8' if windows else 'dom-bin')
    if not path.exists(path0):
        # We're going to create several checkouts of the code. We only actually do
        # an svn checkout for the first one. For the others we just do a folder copy.
        print 'Checking out DOM version ' + domVersion + ' to ' + path0
        os.system('svn co -q ' + sfdom + '/tags/' + domVersion + ' ' + path0)
        if not path.exists(path0):
            print 'svn checkout failed'
            sys.exit(2)
        # Create the plain source checkout
        shutil.copytree(path0, join(outdir, 'dom-source'))
        if windows:
            # Disable debug info and create the vc9 checkout
            disableDebugInfo(path0)
            shutil.copytree(path0, join(outdir, 'dom-vc9'))

def checkFilesExist(dir, files):
    for file in files:
        if not path.exists(join(dir, file)):
            return False
    return True

def createArchive(path, archiveType):
    if archiveType == 'zip':
        return ZipFile(path + '.zip', 'w', zipfile.ZIP_DEFLATED)
    else:
        return TarFileCompat(path + '.tar.gz', 'w', tarfile.TAR_GZIPPED)

def packageRecursive(zip, packagePath, fileExt, archivePrefix):
    def addToArchive(zip, p, packagePath, archivePrefix):
        archivePath = p
        commonIndex = len(path.commonprefix([archivePath, packagePath]))
        archivePath = archivePath[commonIndex:]
        if archivePath[0] == os.sep:
            archivePath = archivePath[1:]
        archivePath = join(archivePrefix, archivePath)
        zip.write(p, archivePath)
    for root, dirs, files in os.walk(packagePath):
        if '.svn' in dirs:
            dirs.remove('.svn')
        for file in files:
            if fileExt == None or path.splitext(file)[1] == fileExt:
                addToArchive(zip, join(root, file), packagePath, archivePrefix)
        # Copy the links to directories, which show up in dirs instead of files
        for dir in dirs:
            if path.islink(join(root, dir)):
                addToArchive(zip, join(root, dir), packagePath, archivePrefix)

def packageDomFilesCommon(zip, codePath):
    if not mac:
        packageRecursive(zip, join(codePath, 'dom', 'include'), '.h', join(archivePrefix, 'include'))
    packageRecursive(zip, join(codePath, 'dom', 'test', 'data'), None, \
                     join(archivePrefix, 'bin', 'domTestData'))
    zip.write(join(codePath, 'dom', 'readme.txt'), join(archivePrefix, 'readme.txt'))

def packageDomFilesVC(zip, codePath):
    domLibPrefix = 'libcollada' + colladaVersionNoDots + 'dom' + domVersionNoDots
    debugBuildPath = join(codePath, 'dom', 'build', 'vc8-' + colladaVersion + '-d')
    releaseBuildPath = join(codePath, 'dom', 'build', 'vc8-' + colladaVersion)
    debugFiles = [domLibPrefix + ext for ext in ['-d.dll', '-d.lib', '-sd.lib']]
    releaseFiles = [domLibPrefix + ext for ext in ['.dll', '.lib', '-s.lib']] + ['domTest.exe']
    if not checkFilesExist(debugBuildPath, debugFiles) \
            or not checkFilesExist(releaseBuildPath, releaseFiles):
        print 'VC build seems to have failed. Some files are missing from ' + \
            join(codePath, 'dom', 'build')
    files = [join(debugBuildPath, file) for file in debugFiles] + \
        [join(releaseBuildPath, file) for file in releaseFiles]
    [zip.write(file, join(archivePrefix, 'bin', path.basename(file))) for file in files]
    packageDomFilesCommon(zip, codePath)

def packageRTFilesVC(zip, codePath):
    rtBinPath = join(codePath, 'rt', 'bin', 'vc8_' + colladaVersion)
    rt = join(rtBinPath, 'COLLADA_RT_VIEWER.exe')
    devil = join(rtBinPath, 'DevIL.dll')
    if not path.exists(rt):
        print 'RT build seems to have failed'
        sys.exit(2)
    zip.write(rt, join(archivePrefix, 'bin', 'COLLADA_RT_VIEWER.exe'))
    zip.write(devil, join(archivePrefix, 'bin', 'DevIL.dll'))

def packageDomFilesLinux(zip, codePath):
    buildPath = join(codePath, 'dom', 'build', 'linux-' + colladaVersion)
    domLibPrefix = 'libcollada' + colladaVersionNoDots + 'dom'
    files = [domLibPrefix + ext for ext in ['.a', '.so', '.so.2', '.so.2.0']]
    files += ['domTest']
    if not checkFilesExist(buildPath, files):
        print 'Build seems to have failed'
        sys.exit(2)
    files = [join(buildPath, file) for file in files]
    [zip.write(file, join(archivePrefix, 'bin', path.basename(file))) for file in files]
    zip.write(join(codePath, 'dom', 'release', 'runDomTest'),
              join(archivePrefix, 'bin', 'runDomTest'))
    packageDomFilesCommon(zip, codePath)

def packageDomFilesPS3(zip, codePath):
    buildPath = join(codePath, 'dom', 'build', 'ps3-' + colladaVersion)
    files = [join(buildPath, file) for file in \
                 ['libcollada' + colladaVersionNoDots + 'dom.a', 'domTest.self']]
    [zip.write(file, join(archivePrefix, 'bin', path.basename(file))) for file in files]
    packageDomFilesCommon(zip, codePath)

def packageDomFilesMac(zip, codePath):
    buildPath = join(codePath, 'dom', 'build', 'mac-' + colladaVersion)
    # Need to change the Mac framework link name in domTest
    frameworkName = 'Collada' + colladaVersionNoDots + 'Dom.framework'
    dylibName = 'Collada' + colladaVersionNoDots + 'Dom'

    domTest = join(buildPath, 'domTest')
    domDylib = join(buildPath, frameworkName, dylibName)
    oldInstallName = popen2.popen2('otool -L ' + domTest + ' | grep ' + frameworkName)[0].read().split(' ')[0]
    newInstallName = join('/Library/Frameworks', frameworkName, 'Versions', domVersion, dylibName)
    if (os.system('install_name_tool -change ' + oldInstallName + ' ' + newInstallName + ' ' + domTest) != 0):
        print "Updating the DOM install name in domTest failed."
        sys.exit(2)
    if (os.system('install_name_tool -id ' + newInstallName + ' ' + domDylib) != 0):
        print "Updating the DOM install name in the DOM framework failed."
        sys.exit(2)

    packageRecursive(zip, join(buildPath, frameworkName), None, join(archivePrefix, frameworkName))

#     tmpDir = join(codePath, '..', 'release-tmp')
#     if not path.exists(tmpDir):
#         os.mkdir(tmpDir)
#     oldDomTest = join(buildPath, 'domTest')
#     newDomTest = join(tmpDir, 'domTest')
#     shutil.copyfile(oldDomTest, newDomTest)
#     oldLinkName = popen2.popen2('otool -L ' + newDomTest + ' | grep ' + frameworkName)[0].read().split(' ')[0]
#     newLinkName = '../' + oldLinkName[oldLinkName.index(frameworkName):]
#     if (os.system('install_name_tool -change ' + oldLinkName + ' ' + newLinkName + ' ' + newDomTest) != 0):
#         print "Updating the DOM link name in domTest failed."
#         sys.exit(2)

#    zip.write(newDomTest, join(archivePrefix, 'bin', 'domTest'))
#    zip.write(join(buildPath, 'domTest'), join(archivePrefix, 'bin', 'domTest'))
    zip.write(domTest, join(archivePrefix, 'bin', 'domTest'))
    zip.write(join(codePath, 'dom', 'release', 'runDomTest'),
              join(archivePrefix, 'bin', 'runDomTest'))
    packageDomFilesCommon(zip, codePath)

# basename: path.splitext(path.basename(archivePath))[0]

def packageSource(opts):
    outdir = getOutdir(opts)
    zip = createArchive(join(outdir, 'colladadom-source'), getArchive(opts))
    packageRecursive(zip, join(outdir, 'dom-source'), None, archivePrefix)

def packageWindows(opts):
    outdir = getOutdir(opts)
    vc8Path = join(outdir, 'dom-vc8')
    vc9Path = join(outdir, 'dom-vc9')

    if getCompiler(opts) in ['vc8', 'all']:
        print 'Doing VC8 build'
        os.spawnl(os.P_WAIT, join(vc8Path, 'dom', 'projects', 'vc8', 'build.bat'))
        zip = createArchive(join(outdir, 'colladadom-vc8'), getArchive(opts))
        packageDomFilesVC(zip, vc8Path)

        # Build RT and add it to the VC8 zip file
        print 'Building RT'
        os.spawnl(os.P_WAIT, join(vc8Path, 'rt', 'projects', 'VC++8', 'build.bat'))
        packageRTFilesVC(zip, vc8Path)

    if getCompiler(opts) in ['vc9', 'all']:
        # This could be automated but I don't have a copy of VC9. Argh!
        print '\n\nTime for the VC9 build. Make sure the project files in the VC9 path are converted,\n' \
            + 'then open the solution and do both a debug and release build. Press enter when\n' \
            + 'the build is finished.'
        raw_input('\nWaiting for user response...')
        print 'ok'

        zip = createArchive(join(outdir, 'colladadom-vc9'), getArchive(opts))
        packageDomFilesVC(zip, vc9Path)

def packageLinux(opts):
    outdir = getOutdir(opts)
    codePath = join(outdir, 'dom-bin')

    print 'Building the DOM'
    makeCmd = 'make' + ' -C ' + join(codePath, 'dom') + ' -k' + \
              ' -j ' + getBuildJobs(opts) + ' conf=release'
    if getPlatform(opts) == 'ps3':
        makeCmd += ' os=ps3'
    os.system(makeCmd)

    ext = '-ps3' if getPlatform(opts) == 'ps3' else '-linux'
    zip = createArchive(join(outdir, 'colladadom' + ext), getArchive(opts))
    if getPlatform(opts) == 'host':
        packageDomFilesLinux(zip, codePath)
    else:
        packageDomFilesPS3(zip, codePath)

def packageMac(opts):
    outdir = getOutdir(opts)
    codePath = join(outdir, 'dom-bin')

    print 'Building the DOM'
    makeCmd = 'make' + ' -C ' + join(codePath, 'dom') + ' -k' + \
              ' -j ' + getBuildJobs(opts) + ' conf=release' + " arch='x86 ppc'"
    os.system(makeCmd)

    zip = createArchive(join(outdir, 'colladadom-mac'), getArchive(opts))
    packageDomFilesMac(zip, codePath)

def buildPackage(opts):
    outdir = getOutdir(opts)
    if not path.exists(outdir):
        os.makedirs(outdir)
    if not path.exists(outdir):
        print "Couldn't create the output directory"
        sys.exit(2)

    setupReleaseCheckout(opts)

    if getPackage(opts) == 'source':
        packageSource(opts)
    elif linux:
        packageLinux(opts)
    elif mac:
        packageMac(opts)
    elif windows:
        packageWindows(opts)

def createOptions():
    return ['binary', 'host', 'zip' if windows else 'tgz', path.abspath('.'), 'all', '1']

def setPackage(opts, package):
    opts[0] = package
def setPlatform(opts, platform):
    opts[1] = platform
def setArchive(opts, archive):
    opts[2] = archive
def setOutdir(opts, outdir):
    opts[3] = outdir
def setCompiler(opts, compiler):
    opts[4] = compiler
def setBuildJobs(opts, jobs):
    opts[5] = jobs

def getPackage(opts):
    return opts[0]
def getPlatform(opts):
    return opts[1]
def getArchive(opts):
    return opts[2]
def getOutdir(opts):
    return opts[3]
def getCompiler(opts):
    return opts[4]
def getBuildJobs(opts):
    return opts[5]

def printUsage():
    print "Options:"
    print "  --package=(source | binary). Build a source package or a package containing binaries."
    print "                               Default is binary."
    print "  --platform=(host | ps3). Only applies when package is set to binary. host (the default)"
    print "                           is the platform you're running on. ps3 binaries can only be"
    print "                           built on Linux."
    print "  --archive=(tgz | zip). Default is tgz on Linux or Mac and zip on Windows."
    print "  --outdir. Set this to a directory to output the files to. Default is the current dir."
    print "  --compiler. Only valid on Windows. Can be set to vc8, vc9, or all (the default)."
    print "  --build-jobs. The number of parallel build jobs make should run. Only valid on Mac"
    print "                and Linux. Default is 1."
    print "  -h, --help. Print this help message."

def main():
    try:
        validShortOpts = 'h'
        validLongOpts = ['package=', 'platform=', 'archive=', 'outdir=', 'compiler=', 'build-jobs=', 'help']
        opts, args = getopt.getopt(sys.argv[1:], validShortOpts, validLongOpts)
    except getopt.GetoptError, err:
        print 'Invalid usage.\n'
        printUsage()
        sys.exit(2)

    options = createOptions()
    for o, a in opts:
        if o in ('-h', '--help'):
            printUsage()
            sys.exit()
        elif o == '--package':
            setPackage(options, a)
        elif o == '--platform':
            setPlatform(options, a)
        elif o == '--archive':
            setArchive(options, a)
        elif o == '--outdir':
            setOutdir(options, a)
        elif o == '--compiler':
            setCompiler(options, a)
        elif o == '--build-jobs':
            setBuildJobs(options, a)

    if not getPackage(options) in ('source', 'binary') or \
            not getPlatform(options) in ('host', 'ps3') or \
            not getArchive(options) in ('zip', 'tgz') or \
            not getCompiler(options) in ('vc8', 'vc9', 'all'):
        print 'Invalid usage.\n'
        printUsage()
        sys.exit(2)

    buildPackage(options)
    print 'Packaging successful!'

main()
