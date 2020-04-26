import sys, os, xml, string, getopt, StringIO
from os import path
from os.path import join, dirname, splitext
from xml.dom import minidom, Node

def replaceInFile(file, origText, newText):
    f = open(file, 'r')
    buffer = StringIO.StringIO()
    for line in f:
        buffer.write(line.replace(origText, newText))
    f.close()
    f = open(file, 'w')
    f.write(buffer.getvalue())

def getChildElement(element, childElementName):
    for child in element.childNodes:
        if child.nodeType == Node.ELEMENT_NODE and child.tagName == childElementName:
            return child
    return None

def getChildFilter(parentFilter, childFilterName):
    for child in parentFilter.childNodes:
        if child.nodeType == Node.ELEMENT_NODE and child.tagName == 'Filter' \
                and child.getAttribute('Name') == childFilterName:
            return child
    return None

def getFileLocationFromFilter(filter, versionTag):
    fileElt = getChildElement(filter, 'File')
    return dirname(fileElt.getAttribute('RelativePath'))

def getFilesInDir(dir, extension):
    result = []
    for file in os.listdir(dir):
        if splitext(file)[1] == extension:
            result.append(file)
    return result

def removeAllChildNodes(node):
    children = list(node.childNodes)
    [node.removeChild(child) for child in children]
    
def convertFiles(vcprojFile, doc, filterNode, fileExtension, fromVersion, toVersion):
    dir = getFileLocationFromFilter(filterNode, fromVersion).replace(fromVersion, toVersion)
    removeAllChildNodes(filterNode)
    files = getFilesInDir(join(dirname(vcprojFile), dir), fileExtension)
    for file in files:
        filterNode.appendChild(doc.createTextNode("\n\t\t\t\t"))
        fileElt = filterNode.appendChild(doc.createElement('File'))
        fileElt.setAttribute('RelativePath', join(dir, file))
    filterNode.appendChild(doc.createTextNode("\n\t\t\t"))
    
def vcprojChangeColladaVersionRecursive(vcprojFile, doc, node, fromVersion, toVersion):
    if node.nodeType == Node.ELEMENT_NODE and node.tagName == 'Configuration':
        if node.getAttributeNode('OutputDirectory'):
            outDir = node.getAttribute('OutputDirectory')
            node.setAttribute('OutputDirectory', outDir.replace(fromVersion, toVersion))
        if node.getAttributeNode('IntermediateDirectory'):
            intDir = node.getAttribute('IntermediateDirectory')
            node.setAttribute('IntermediateDirectory', intDir.replace(fromVersion, toVersion))
    elif node.nodeType == Node.ELEMENT_NODE and node.tagName == 'Tool':
        if node.getAttribute('Name') == 'VCCLCompilerTool':
            incDirs = node.getAttribute('AdditionalIncludeDirectories')
            node.setAttribute('AdditionalIncludeDirectories', incDirs.replace(fromVersion, toVersion))
    elif node.nodeType == Node.ELEMENT_NODE and node.tagName == 'Filter' \
            and node.getAttribute('Name') == fromVersion:
        node.setAttribute('Name', toVersion)
        convertFiles(vcprojFile, doc, getChildFilter(node, 'Header Files'), '.h', fromVersion, toVersion)
        convertFiles(vcprojFile, doc, getChildFilter(node, 'Source Files'), '.cpp', fromVersion, toVersion)
    #TODO handle domTest
    [vcprojChangeColladaVersionRecursive(vcprojFile, doc, child, fromVersion, toVersion) for child in node.childNodes]

def vcprojChangeColladaVersion(vcprojFrom, vcprojTo, fromVersion, toVersion):
    root = minidom.parse(vcprojFrom)
    if not root:
        return False
    vcprojChangeColladaVersionRecursive(vcprojFrom, root, root, fromVersion, toVersion)
    root.writexml(open(vcprojTo, 'w'))
    fromVersionNoDot = fromVersion.replace('.', '')
    toVersionNoDot = toVersion.replace('.', '')
    replaceInFile(vcprojTo, 'libcollada' + fromVersionNoDot, 'libcollada' + toVersionNoDot)
    return True

def changeColladaVersion(fromDir, toDir, fromVersion, toVersion):
    if not path.exists(toDir):
        os.mkdir(toDir)
    for file in os.listdir(fromDir):
        if splitext(file)[1] == '.vcproj':
            vcprojChangeColladaVersion(join(fromDir, file), join(toDir, file), fromVersion, toVersion)

if len(sys.argv) < 5:
    print 'failed'
changeColladaVersion(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
