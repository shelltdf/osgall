import sys, os, xml, string, getopt, StringIO
from os import path
from os.path import join
from xml.dom import minidom, Node

def slnToVC9(vc8Sln, vc9Sln):
    vc8File = open(vc8Sln, 'r')
    vc9File = open(vc9Sln, 'w')
    if not vc8File or not vc9File:
        return False
    for line in vc8File:
        if line.find('Microsoft Visual Studio Solution File, Format Version 9.00') != -1:
            vc9File.write('Microsoft Visual Studio Solution File, Format Version 10.00\n')
        elif line.find('# Visual Studio 2005') != -1:
            vc9File.write('# Visual Studio 2008\n')
        else:
            vc9File.write(line)
    return True

def vcprojToVC9Recursive(node):
    if node.nodeType == Node.ELEMENT_NODE and node.tagName == 'VisualStudioProject':
        attr = node.getAttributeNode('Version')
        if attr:
            attr.nodeValue = '9.00'
        else:
            return False
        node.setAttribute('TargetFrameworkVersion', '131072')
    if node.nodeType == Node.ELEMENT_NODE and node.tagName == 'Tool':
        if node.getAttribute('Name') == 'VCLinkerTool':
            node.setAttribute('RandomizedBaseAddress', '1')
            node.setAttribute('DataExecutionPrevention', '0')
    if node.nodeType == Node.ELEMENT_NODE and node.tagName == 'Tool':
        if node.getAttribute('Name') == 'VCWebDeploymentTool':
            node.parentNode.removeChild(node)
    if node.nodeType == Node.ELEMENT_NODE and node.tagName == 'Tool':
        if node.getAttribute('Name') == 'VCCLCompilerTool':
            if node.getAttributeNode('Detect64BitPortabilityProblems'):
                node.setAttribute('Detect64BitPortabilityProblems', 'false')
    map(vcprojToVC9Recursive, node.childNodes)
    
def vcprojToVC9(vc8Proj, vc9Proj):
    root = minidom.parse(vc8Proj)
    if not root:
        return False
    vcprojToVC9Recursive(root)
    root.writexml(open(vc9Proj, 'w'))
    return True

def toVC9(vc8Path, vc9Path):
    if not path.exists(vc9Path):
        os.mkdir(vc9Path)
    for path_ in os.listdir(vc8Path):
        if path.splitext(path_)[1] == '.sln':
            if not slnToVC9(join(vc8Path, path_), join(vc9Path, path_)):
                return False
        elif path.splitext(path_)[1] == '.vcproj':
            if not vcprojToVC9(join(vc8Path, path_), join(vc9Path, path_)):
                return False
    return True

def vcprojConvertVC8Tags(vc9Proj):
    vc9File = open(vc9Proj, 'r')
    vc9Out = StringIO.StringIO()
    if not vc9File or not vc9Out:
        return False
    for line in vc9File:
        line = line.replace('vc8', 'vc9')
        line = line.replace('VC8', 'VC9')
        vc9Out.write(line)
    vc9File.close()
    vc9File = open(vc9Proj, 'w')
    vc9File.write(vc9Out.getvalue())
    return True
    
def convertVC8Tags(dir):
    if not path.exists(dir):
        return False
    for path_ in os.listdir(dir):
        if path.splitext(path_)[1] == '.vcproj':
            if not vcprojConvertVC8Tags(join(dir, path_)):
                return False
    return True


if len(sys.argv) < 2 or not path.exists(sys.argv[1]):
    print 'failed'
vc8Path = sys.argv[1]
vc9Path = join(vc8Path, '..', 'vc9')
if len(sys.argv) > 2:
    vc9Path = sys.argv[2]
if not toVC9(vc8Path, vc9Path):
    print 'failed'
if len(sys.argv) > 3 and sys.argv[3] == 'convertVC8Tags':
    if not convertVC8Tags(vc9Path):
        print 'failed'
