#!/usr/bin/python

# Filename : BuildEngine.py
# Version : 1.0

import sys
import uuid
import os

def main_is_frozen():
   return (hasattr(sys, "frozen") or # new py2exe
           hasattr(sys, "importers") # old py2exe
           or imp.is_frozen("__main__")) # tools/freeze

def get_main_dir():
   if main_is_frozen():
       return os.path.dirname(sys.executable)
   return os.path.dirname(os.path.realpath(__file__))

SCRIPT_ROOT = get_main_dir()

# Global Configs
vsName = 'Visual Studio 2013'
slnName = 'Demi'
evaluateLines = False
generateProjs = False
generateExternal = False
clean = False
scriptDir = os.path.dirname(os.path.realpath(SCRIPT_ROOT))
rootDir = os.path.normpath(os.path.join(scriptDir, "../../"))
slnFileName = os.path.normpath(rootDir + '/build/' + slnName + '.sln')
projsDir = os.path.normpath(rootDir + '/build/vsprojs')
srcDir = os.path.normpath(rootDir + '/src')
externalIncDir = os.path.normpath(rootDir + '/external/include')
externalLibDbgDir = os.path.normpath(rootDir + '/external/lib/Debug')
externalLibRelDir = os.path.normpath(rootDir + '/external/lib/Release')
binDebugDir = os.path.normpath(rootDir + '/bin/debug')
binReleaseDir = os.path.normpath(rootDir + '/bin/release')
libDebugDir = os.path.normpath(rootDir + '/lib/debug')
libReleaseDir = os.path.normpath(rootDir + '/lib/release')
objDebugDir = os.path.normpath(rootDir + '/obj/debug')
objReleaseDir = os.path.normpath(rootDir + '/obj/release')

generalPreDefDebug = 'WIN32;_CRT_SECURE_NO_WARNINGS;DEBUG;_DEBUG'
generalPreDefRelease = 'WIN32;_CRT_SECURE_NO_WARNINGS;NDEBUG'
generalWarningLevel = 'Level3'
generalAdditionOpt = '-Zm200  -FS'
ln = '\n'

# DirectX SDK Path
dxSDKDefaultRoot = 'C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/'
dxSDKIncDir = os.path.normpath(os.getenv('DXSDK_DIR', dxSDKDefaultRoot) + '/Include')
dxSDKLibDir = os.path.normpath(os.getenv('DXSDK_DIR', dxSDKDefaultRoot) + '/Lib/x86')

# arguments
if len(sys.argv) <= 1:
    print 'No valid arguments'
    sys.exit()

for arg in sys.argv:
    if arg == '-linenumber':
        evaluateLines = True
    if arg == '-clean':
        clean = True
        generateProjs = False
        generateExternal = False
    if arg == '-gen':
        generateProjs = True
    if arg == '-vs2010':
        vsName = 'Visual Studio 2010'
    if arg == '-vs2012':
        vsName = 'Visual Studio 2012'
    if arg == '-vs2013':
        vsName = 'Visual Studio 2013'
    if arg == '-external':
        generateProjs = False
        if clean is False:
			generateExternal = True
        slnName = 'DemiExternal'
        slnFileName = os.path.normpath(rootDir + '/external/build/' + slnName + '.sln')
        projsDir = os.path.normpath(rootDir + '/external/build/vsprojs')
        binDebugDir = os.path.normpath(rootDir + '/bin/debug')
        binReleaseDir = os.path.normpath(rootDir + '/external/bin/release')
        libDebugDir = os.path.normpath(rootDir + '/external/lib/debug')
        libReleaseDir = os.path.normpath(rootDir + '/external/lib/release')
        objDebugDir = os.path.normpath(rootDir + '/external/obj/debug')
        objReleaseDir = os.path.normpath(rootDir + '/external/obj/release')
        srcDir = os.path.normpath(rootDir + '/external/src')

# random GUID string
def genGUID():
    guid = uuid.uuid4()
    return str(guid).upper()

# wrap the guid string
def getGUIDStr(guid):
    return '\"{' + guid + '}\"'

def getFileLines(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def removeFiles(path,pattern,maxdepth=100):
    cpath = path.count(os.sep)
    for r,d,f in os.walk(path):
        if r.count(os.sep) - cpath < maxdepth:
            for name in f:
                if not len(pattern) or os.path.splitext(name)[1] in pattern:
                    os.remove(os.path.join(r, name))
            #for name in d:
                #os.rmdir(os.path.join(r, name))

def cleanup():
    print('Cleanning..')
    removeFiles(os.path.normpath(rootDir + '/lib'), [])
    removeFiles(os.path.normpath(rootDir + '/bin'), [])
    removeFiles(os.path.normpath(rootDir + '/obj'), [])
    removeFiles(os.path.normpath(rootDir + '/build'), [], 1)
    removeFiles(os.path.normpath(rootDir + '/build/vsprojs'), [],1)

def addHeadFile(proj, directoryName,filesInDirectory):
    file = proj.file
    for fname in filesInDirectory:
        if directoryName in proj.exludeDirs:
            continue
        fpath = os.path.join(directoryName, fname)
        if not os.path.isdir(fpath):
            if os.path.splitext(fname)[1] in ['.h','.hpp']:
                if evaluateLines:
                    proj.lines += getFileLines(fpath)
                file.write('    <ClInclude Include="' + fpath + '" />' + ln)

def addSrcFile(proj, directoryName,filesInDirectory):
    file = proj.file
    for fname in filesInDirectory:
        if directoryName in proj.exludeDirs:
            continue
        fpath = os.path.join(directoryName, fname)
        if not os.path.isdir(fpath):
            if os.path.splitext(fname)[1] in ['.c','.cpp','.cc']:
                if evaluateLines:
                    proj.lines += getFileLines(fpath)

                if fname.lower() == proj.pchSrc.lower():
                    file.write('    <ClCompile Include="' + fpath + '">' + ln)
                    file.write('      <PrecompiledHeader Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">Create</PrecompiledHeader>' + ln)
                    file.write('      <PrecompiledHeader Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">Create</PrecompiledHeader>' + ln)
                    file.write('    </ClCompile>' + ln)
                else:
                    file.write('    <ClCompile Include="' + fpath + '" />' + ln)

# vs version
class VSVer:
    def __init__(self,name):
        self.name = name
        self.toolsVersion = ''
        self.formatVersion = ''
        self.vsVersion = ''
        self.projFileVersion = ''
        self.platformToolset = ''

    def init(self):
        self.checkVSVersion(self.name)
        self.setProp()

    def checkVSVersion(self,name):
        if not name in ['Visual Studio 2010','Visual Studio 2012','Visual Studio 2013']:
            print('Error! Unsupported visual studio version: ' + name)
            sys.exit()
        else:
            print('Target Visual Studio Version: ' + name)

    def setProp(self):
        if self.name == 'Visual Studio 2010':
            self.toolsVersion = '4.0'
            self.formatVersion = '11.00'
            self.vsVersion = '10.0'
            self.projFileVersion = '10.0.30319.1'
            self.platformToolset = '<PlatformToolset>v100</PlatformToolset>'
        elif self.name == 'Visual Studio 2012':
            self.toolsVersion = '4.0'
            self.formatVersion = '12.00'
            self.vsVersion = '11.0'
            self.projFileVersion = '10.0.30319.1'
            self.platformToolset = '<PlatformToolset>v110</PlatformToolset>'
        elif self.name == 'Visual Studio 2013':
            self.toolsVersion = '12.0'
            self.formatVersion = '13.00'
            self.vsVersion = '12.0'
            self.projFileVersion = '10.0.30319.1'
            self.platformToolset = '<PlatformToolset>v120</PlatformToolset>'

vsVer = VSVer(vsName)

# project
class Project:
    def __init__(self,name,folder,type):
        self.name = name
        self.guid = genGUID()
        self.folder = folder
        self.srcDir = os.path.normpath(srcDir + folder)
        self.projFile = os.path.normpath(projsDir + '/' + name + '.vcxproj')
        self.type = type
        self.extraPreDefinesDbg = generalPreDefDebug + ';'
        self.extraPreDefinesRel = generalPreDefRelease + ';'
        self.addIncDirDbg = self.srcDir + ';'
        self.addIncDirRel = self.srcDir + ';'
        self.addLibDirDbg = libDebugDir + ';'
        self.addLibDirRel = libReleaseDir + ';'
        self.dependsLibsDbg = ''
        self.dependsLibsRel = ''
        self.pch = 'Use'
        self.pchHead = ''
        self.pchSrc = ''
        self.subSystem = 'Windows'
        self.file = None
        self.lines = 0
        self.outputExt = ''
        self.dependsProjs = []
        self.exludeDirs = []
        self.onlyBuildInc = []
        self.onlyBuildSrc = []

        if self.type != 'StaticLibrary':
            self.outDebugDir = os.path.normpath(binDebugDir)
            self.outReleaseDir = os.path.normpath(binDebugDir)
        else:
            self.outDebugDir = os.path.normpath(libDebugDir)
            self.outReleaseDir = os.path.normpath(libReleaseDir)

    def appendIncludeDir(self,dir):
        absDir = ''
        if os.path.isabs(dir):
            absDir = dir
        else:
            absDir = os.path.realpath(self.srcDir + '/' + dir)
        self.addIncDirDbg += absDir + ';'
        self.addIncDirRel += absDir + ';'

    def appendDebugLibDir(self,dir):
        absDir = ''
        if os.path.isabs(dir):
            absDir = dir
        else:
            absDir = os.path.realpath(self.srcDir + '/' + dir)
        self.addLibDirDbg += absDir + ';'

    def appendReleaseLibDir(self,dir):
        absDir = ''
        if os.path.isabs(dir):
            absDir = dir
        else:
            absDir = os.path.realpath(self.srcDir + '/' + dir)
        self.addLibDirRel += absDir + ';'

    def appendDependsLibDbg(self,dir):
        self.dependsLibsDbg += dir + ';'

    def appendDependsLibRel(self,dir):
        self.dependsLibsRel += dir + ';'

    def setExtraPreDefines(self,defines):
        self.extraPreDefinesDbg += defines + ';'
        self.extraPreDefinesRel += defines + ';'

    def writeProjConfig(self):
        file = self.file
        file.write('  <ItemGroup Label="ProjectConfigurations">' + ln)
        file.write('    <ProjectConfiguration Include="Debug|Win32">' + ln)
        file.write('      <Configuration>Debug</Configuration>' + ln)
        file.write('      <Platform>Win32</Platform>' + ln)
        file.write('    </ProjectConfiguration>' + ln)
        file.write('    <ProjectConfiguration Include="Release|Win32">' + ln)
        file.write('      <Configuration>Release</Configuration>' + ln)
        file.write('      <Platform>Win32</Platform>' + ln)
        file.write('    </ProjectConfiguration>' + ln)
        file.write('  </ItemGroup>' + ln)

    def writeGlobalPropGroup(self):
        file = self.file
        file.write('  <PropertyGroup Label="Globals">' + ln)
        file.write('    <ProjectGuid>{' + self.guid + '}</ProjectGuid>' + ln)
        file.write('    <RootNamespace>' + self.name + '</RootNamespace>' + ln)
        file.write('    <Keyword>Win32Proj</Keyword>' + ln)
        file.write('  </PropertyGroup>' + ln)

    def writeConfigPropGroup(self):
        file = self.file
        file.write('<Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />' + ln)
        file.write('  <PropertyGroup Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'" Label="Configuration">' + ln)
        file.write('    <ConfigurationType>' + self.type + '</ConfigurationType>' + ln)
        file.write('    <UseDebugLibraries>true</UseDebugLibraries>' + ln)
        file.write('    <CharacterSet>MultiByte</CharacterSet>' + ln)
        file.write('    ' + vsVer.platformToolset + ln)
        file.write('  </PropertyGroup>' + ln)
        file.write('  <PropertyGroup Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'" Label="Configuration">' + ln)
        file.write('    <ConfigurationType>' + self.type + '</ConfigurationType>' + ln)
        file.write('    <UseDebugLibraries>false</UseDebugLibraries>' + ln)
        file.write('    <CharacterSet>MultiByte</CharacterSet>' + ln)
        file.write('    ' + vsVer.platformToolset + ln)
        file.write('  </PropertyGroup>' + ln)

    def writeImportGroup(self):
        file = self.file
        file.write('  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />' + ln)
        file.write('  <ImportGroup Label="ExtensionSettings">' + ln)
        file.write('  </ImportGroup>' + ln)
        file.write('  <ImportGroup Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'" Label="PropertySheets">' + ln)
        file.write('    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists(\'$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\')" Label="LocalAppDataPlatform" />' + ln)
        file.write('  </ImportGroup>' + ln)
        file.write('  <ImportGroup Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'" Label="PropertySheets">' + ln)
        file.write('    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists(\'$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\')" Label="LocalAppDataPlatform" />' + ln)
        file.write('  </ImportGroup>' + ln)

    def writeOutputProp(self):
        file = self.file
        file.write('  <PropertyGroup Label="UserMacros" />' + ln)
        file.write('  <PropertyGroup>' + ln)
        file.write('    <_ProjectFileVersion>' + vsVer.projFileVersion + '</_ProjectFileVersion>' + ln)
        file.write('    <OutDir Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">' + self.outDebugDir + '\\</OutDir>' + ln)
        file.write('    <IntDir Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">' + os.path.normpath(objDebugDir+self.folder) + '\\</IntDir>' + ln)
        file.write('    <TargetName Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">' + self.name + '_d' + '</TargetName>' + ln)
        file.write('    <IgnoreImportLibrary Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">false</IgnoreImportLibrary>' + ln)
        file.write('    <LinkIncremental Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">false</LinkIncremental>' + ln)
        file.write('    <OutDir Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">' + self.outReleaseDir + '\\</OutDir>' + ln)
        file.write('    <IntDir Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">' + os.path.normpath(objReleaseDir+self.folder) + '\\</IntDir>' + ln)
        file.write('    <TargetName Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">' + self.name + '</TargetName>' + ln)
        file.write('    <IgnoreImportLibrary Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">false</IgnoreImportLibrary>' + ln)
        file.write('    <LinkIncremental Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">false</LinkIncremental>' + ln)
        file.write('  </PropertyGroup>' + ln)

    def writeItemConfigsDbg(self):
        file = self.file
        file.write('<ItemDefinitionGroup Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">' + ln)
        file.write('    <ClCompile>' + ln)
        file.write('      <AdditionalOptions>' + generalAdditionOpt + ' %(AdditionalOptions)</AdditionalOptions>' + ln)
        file.write('      <Optimization>Disabled</Optimization>' + ln)
        file.write('      <AdditionalIncludeDirectories>' + self.addIncDirDbg + '%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>' + ln)
        file.write('      <PreprocessorDefinitions>' + self.extraPreDefinesDbg + '%(PreprocessorDefinitions)</PreprocessorDefinitions>' + ln)
        file.write('      <MinimalRebuild>true</MinimalRebuild>' + ln)
        file.write('      <BasicRuntimeChecks>EnableFastChecks</BasicRuntimeChecks>' + ln)
        file.write('      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' + ln)
        file.write('      <FunctionLevelLinking>true</FunctionLevelLinking>' + ln)
        file.write('      <WarningLevel>' + generalWarningLevel + '</WarningLevel>' + ln)

        if self.pch == 'Use':
            file.write('      <PrecompiledHeader>' + self.pch + '</PrecompiledHeader>' + ln)
            file.write('      <PrecompiledHeaderFile>' + self.pchHead + '</PrecompiledHeaderFile>' + ln)
            file.write('      <ForcedIncludeFiles>' + self.pchHead + '</ForcedIncludeFiles>' + ln)

        file.write('      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>' + ln)
        file.write('      <ProgramDataBaseFileName>$(OutDir)' + self.name + '_d.pdb</ProgramDataBaseFileName>' + ln)
        file.write('    </ClCompile>' + ln)
        file.write('    <ResourceCompile>' + ln)
        file.write('      <PreprocessorDefinitions>' + self.extraPreDefinesDbg + '%(PreprocessorDefinitions)</PreprocessorDefinitions>' + ln)
        file.write('      <AdditionalIncludeDirectories>' + self.addIncDirDbg + '%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>' + ln)
        file.write('    </ResourceCompile>' + ln)
        file.write('    <Link>' + ln)
        file.write('      <SubSystem>' + self.subSystem + '</SubSystem>' + ln)
        file.write('      <GenerateDebugInformation>true</GenerateDebugInformation>' + ln)
        file.write('      <AdditionalDependencies>' + self.dependsLibsDbg + '%(AdditionalDependencies)</AdditionalDependencies>' + ln)
        file.write('      <OutputFile>$(OutDir)' + self.name + '_d' + self.outputExt + '</OutputFile>' + ln)
        file.write('      <AdditionalLibraryDirectories>' + self.addLibDirDbg + '%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>' + ln)
        file.write('      <ImportLibrary>' + libDebugDir + '\\' + self.name + '_d.lib</ImportLibrary>' + ln)
        file.write('    </Link>' + ln)
        file.write('  </ItemDefinitionGroup>' + ln)

    def writeItemConfigsRel(self):
        file = self.file
        file.write('<ItemDefinitionGroup Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">' + ln)
        file.write('    <ClCompile>' + ln)
        file.write('      <AdditionalOptions>' + generalAdditionOpt + ' %(AdditionalOptions)</AdditionalOptions>' + ln)
        file.write('      <Optimization>MaxSpeed</Optimization>' + ln)
        file.write('      <AdditionalIncludeDirectories>' + self.addIncDirRel + '%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>' + ln)
        file.write('      <PreprocessorDefinitions>' + self.extraPreDefinesRel + '%(PreprocessorDefinitions)</PreprocessorDefinitions>' + ln)
        file.write('      <MinimalRebuild>false</MinimalRebuild>' + ln)
        file.write('      <StringPooling>true</StringPooling>' + ln)
        file.write('      <IntrinsicFunctions>true</IntrinsicFunctions>' + ln)
        file.write('      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' + ln)
        file.write('      <FunctionLevelLinking>true</FunctionLevelLinking>' + ln)

        if self.pch == 'Use':
            file.write('      <PrecompiledHeader>' + self.pch + '</PrecompiledHeader>' + ln)
            file.write('      <PrecompiledHeaderFile>' + self.pchHead + '</PrecompiledHeaderFile>' + ln)
            file.write('      <ForcedIncludeFiles>' + self.pchHead + '</ForcedIncludeFiles>' + ln)

        file.write('      <WarningLevel>' + generalWarningLevel + '</WarningLevel>' + ln)
        file.write('      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>' + ln)
        file.write('      <ProgramDataBaseFileName>$(OutDir)' + self.name + '.pdb</ProgramDataBaseFileName>' + ln)
        file.write('    </ClCompile>' + ln)
        file.write('    <ResourceCompile>' + ln)
        file.write('      <PreprocessorDefinitions>' + self.extraPreDefinesRel + '%(PreprocessorDefinitions)</PreprocessorDefinitions>' + ln)
        file.write('      <AdditionalIncludeDirectories>' + self.addIncDirRel + '%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>' + ln)
        file.write('    </ResourceCompile>' + ln)
        file.write('    <Link>' + ln)
        file.write('      <SubSystem>' + self.subSystem + '</SubSystem>' + ln)
        file.write('      <GenerateDebugInformation>true</GenerateDebugInformation>' + ln)
        file.write('      <EnableCOMDATFolding>true</EnableCOMDATFolding>' + ln)
        file.write('      <OptimizeReferences>true</OptimizeReferences>' + ln)
        file.write('      <AdditionalDependencies>' + self.dependsLibsRel + '%(AdditionalDependencies)</AdditionalDependencies>' + ln)
        file.write('      <OutputFile>$(OutDir)' + self.name + self.outputExt + '</OutputFile>' + ln)
        file.write('      <AdditionalLibraryDirectories>' + self.addLibDirRel + '%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>' + ln)
        file.write('      <ImportLibrary>' + libReleaseDir + '\\' + self.name + '.lib</ImportLibrary>' + ln)
        file.write('    </Link>' + ln)
        file.write('  </ItemDefinitionGroup>' + ln)

    def writeCodeFiles(self):
        file = self.file
        if not self.onlyBuildInc and not self.onlyBuildInc:
            # head files
            file.write('  <ItemGroup>' + ln)
            os.path.walk(self.srcDir, addHeadFile, self)
            file.write('  </ItemGroup>' + ln)

            # source files
            file.write('  <ItemGroup>' + ln)
            os.path.walk(self.srcDir, addSrcFile, self)
            file.write('  </ItemGroup>' + ln)
        else:
            # head files
            file.write('  <ItemGroup>' + ln)
            for h in self.onlyBuildInc:
                file.write('    <ClInclude Include="' + os.path.normpath(srcDir + h) + '" />' + ln)
            file.write('  </ItemGroup>' + ln)

            # source files
            file.write('  <ItemGroup>' + ln)
            for h in self.onlyBuildSrc:
                fname = os.path.basename(h)
                if fname.lower() == self.pchSrc.lower():
                    file.write('    <ClCompile Include="' + os.path.normpath(srcDir + h) + '">' + ln)
                    file.write('      <PrecompiledHeader Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">Create</PrecompiledHeader>' + ln)
                    file.write('      <PrecompiledHeader Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">Create</PrecompiledHeader>' + ln)
                    file.write('    </ClCompile>' + ln)
                else:
                    file.write('    <ClCompile Include="' + os.path.normpath(srcDir + h) + '" />' + ln)
            file.write('  </ItemGroup>' + ln)

    def writeMisc(self):
        file = self.file
        file.write('  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />' + ln)
        file.write('  <ImportGroup Label="ExtensionTargets">' + ln)
        file.write('  </ImportGroup>' + ln)

    def writeProjFile(self):
        self.outputExt = '.exe' if self.type == 'Application' else '.dll'
        print("Writing project file : " + self.projFile)
        with open(self.projFile,'w+') as file:
            self.file = file
            file.write('<?xml version="1.0" encoding="utf-8"?>' + ln)
            file.write('<Project DefaultTargets="Build" ToolsVersion="' + vsVer.toolsVersion + '" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">' + ln)
            self.writeProjConfig()
            self.writeGlobalPropGroup()
            self.writeConfigPropGroup()
            self.writeImportGroup()
            self.writeOutputProp()
            self.writeItemConfigsDbg()
            self.writeItemConfigsRel()
            self.writeCodeFiles()
            self.writeMisc()
            file.write('</Project>' + ln)
            file.close()
            self.file = None
            if evaluateLines:
                print('Project, '+self.name+' lines:')
                print(self.lines)

# solution folder
class SlnFolder:
    def __init__(self,name):
        self.name = name
        self.guid0 = genGUID()
        self.guid1 = genGUID()

# solution
class Solution:
    def __init__(self):
        self.projects = []
        self.folders = []

    def addProject(self,name,folder,type):
        proj = Project(name,folder,type)
        self.projects.append(proj)
        return proj

    def addSlnFolder(self,name):
        self.folders.append(SlnFolder(name))

    def writeProject(self,file,project):
        file.write('Project(' + getGUIDStr(self.guid) + ') = \"' + project.name + '\", ')
        file.write('\"' + project.projFile + '\",' + getGUIDStr(project.guid) + ln)
        # write project dependencies
        if len(project.dependsProjs):
            file.write('\tProjectSection(ProjectDependencies) = postProject' + ln)
            for dep in project.dependsProjs:
                file.write('\t\t{' + dep.guid + '} = {' + dep.guid + '}' + ln)
            file.write('\tEndProjectSection' + ln)
        file.write('EndProject' + ln)

    def writeFolder(self,file,folder):
        file.write('Project(' + getGUIDStr(folder.guid0) + ') = "' + folder.name + '", '"" + folder.name + '", ' + getGUIDStr(folder.guid1) + ln)
        file.write('EndProject' + ln)

    def writeGlobal(self,file):
        file.write('Global\n')
        file.write('\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n')
        file.write('\t\tDebug|Win32 = Debug|Win32\n')
        file.write('\t\tRelease|Win32 = Release|Win32\n')
        file.write('\tEndGlobalSection\n')
        file.write('\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n')
        for proj in self.projects:
            file.write('\t\t{'+proj.guid+'}.Debug|Win32.ActiveCfg = Debug|Win32\n')
            file.write('\t\t{'+proj.guid+'}.Debug|Win32.Build.0 = Debug|Win32\n')
            file.write('\t\t{'+proj.guid+'}.Release|Win32.ActiveCfg = Release|Win32\n')
            file.write('\t\t{'+proj.guid+'}.Release|Win32.Build.0 = Release|Win32\n')
        file.write('\tEndGlobalSection\n')
        file.write('\tGlobalSection(SolutionProperties) = preSolution\n')
        file.write('\t\tHideSolutionNode = FALSE\n')
        file.write('\tEndGlobalSection\n')
        file.write('EndGlobal\n')

    def generateSln(self):
        self.guid = genGUID()
        with open(slnFileName,'w+') as slnFile:
            slnFile.write('\nMicrosoft Visual Studio Solution File, Format Version ' + vsVer.formatVersion + '\n')
            slnFile.write('# ' + vsVer.name + '\n')
            for proj in self.projects:
                self.writeProject(slnFile,proj)
            #for folder in self.folders:
            #    self.writeFolder(slnFile,folder)
            self.writeGlobal(slnFile)
            slnFile.close()

    def generateProjs(self):
        for proj in self.projects:
            proj.writeProjFile()

def generateExternalProjects():
    print('Generating Demi3D external project files..')

    vsVer.init()

    # make the DemiExternal.sln file
    sln = Solution()

    projOIS = sln.addProject('OIS','/ois','DynamicLibrary')
    projOIS.setExtraPreDefines('OIS_NONCLIENT_BUILD;OIS_DYNAMIC_LIB;OIS_EXPORTS')
    projOIS.appendIncludeDir('./includes')
    projOIS.pch = ''
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/includes/iphone'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/includes/linux'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/includes/mac'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/includes/SDL'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/src/extras/LIRC'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/src/iphone'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/src/linux'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/src/mac'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/src/SDL'))
    projOIS.exludeDirs.append(os.path.normpath(srcDir + '/ois/src/win32/extras/WiiMote'))
    projOIS.appendIncludeDir(dxSDKIncDir)
    projOIS.appendDebugLibDir(dxSDKLibDir)
    projOIS.appendReleaseLibDir(dxSDKLibDir)
    projOIS.appendDependsLibDbg('kernel32.lib;user32.lib;gdi32.lib;winspool.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;comdlg32.lib;advapi32.lib;dinput8.lib;dxguid.lib')
    projOIS.appendDependsLibRel('kernel32.lib;user32.lib;gdi32.lib;winspool.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;comdlg32.lib;advapi32.lib;dinput8.lib;dxguid.lib')

    projFreetype = sln.addProject('Freetype','/freetype','StaticLibrary')
    projFreetype.setExtraPreDefines('FT2_BUILD_LIBRARY')
    projFreetype.appendIncludeDir('./include')
    projFreetype.pch = ''
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/config/ftconfig.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/config/ftheader.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/config/ftmodule.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/config/ftoption.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/config/ftstdlib.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/freetype.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftadvanc.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftbbox.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftbdf.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftbitmap.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftbzip2.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftcache.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftchapters.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftcid.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/fterrdef.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/fterrors.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftgasp.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftglyph.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftgxval.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftgzip.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftimage.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftincrem.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftlcdfil.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftlist.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftlzw.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftmac.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftmm.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftmodapi.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftmoderr.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftotval.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftoutln.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftpfr.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftrender.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftsizes.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftsnames.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftstroke.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftsynth.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftsystem.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/fttrigon.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/fttypes.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftwinfnt.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ftxf86.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/t1tables.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ttnameid.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/tttables.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/tttags.h')
    projFreetype.onlyBuildInc.append('/freetype/include/freetype/ttunpat.h')
    projFreetype.onlyBuildInc.append('/freetype/include/ft2build.h')

    projFreetype.onlyBuildSrc.append('/freetype/src/autofit/autofit.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftbase.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftbbox.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftbitmap.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftfstype.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftgasp.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftglyph.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftinit.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftmm.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftpfr.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftstroke.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftsynth.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftsystem.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/fttype1.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/base/ftwinfnt.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/bdf/bdf.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/bzip2/ftbzip2.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/cache/ftcache.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/cff/cff.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/cid/type1cid.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/gzip/ftgzip.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/lzw/ftlzw.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/pcf/pcf.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/pfr/pfr.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/psaux/psaux.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/pshinter/pshinter.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/psnames/psmodule.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/raster/raster.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/sfnt/sfnt.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/smooth/smooth.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/truetype/truetype.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/type1/type1.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/type42/type42.c')
    projFreetype.onlyBuildSrc.append('/freetype/src/winfonts/winfnt.c')

    coreProjs = [projOIS, projFreetype]

    sln.generateSln()
    sln.generateProjs()

def generate():
    print('Generating Demi3D project files..')

    vsVer.init()

    # make the Demi.sln file
    sln = Solution()

    def addAppProj(name,folder,depends):
        proj = sln.addProject(name,folder,'Application')
        proj.appendIncludeDir(srcDir + '/demi/misc')
        proj.appendIncludeDir(srcDir + '/demi/gfx')
        proj.appendIncludeDir(srcDir + '/demi/ext')
        proj.appendIncludeDir(srcDir + '/demi/gui')
        proj.appendIncludeDir(os.path.normpath(rootDir + '/external/src/ois/includes'))
        #proj.appendIncludeDir(srcDir + '/demi/drvD3D9')
        proj.appendIncludeDir(dxSDKIncDir)
        proj.appendDependsLibDbg('DiMisc_d.lib;DiGfx_d.lib;DiExt_d.lib;')
        proj.appendDependsLibRel('DiMisc.lib;DiGfx.lib;DiExt.lib;')
        proj.pchHead = 'DiPch.h'
        proj.pchSrc = 'DiPch.cpp'
        proj.subSystem = 'Console'
        proj.dependsProjs = depends
        return proj

    #sln.addSlnFolder('Core')
    #sln.addSlnFolder('Samples')
    #sln.addSlnFolder('Tests')
    #sln.addSlnFolder('Tools')

    ####################################################################################################################
    # Main project
    projMisc = sln.addProject('DiMisc','/demi/misc','DynamicLibrary')
    projMisc.setExtraPreDefines('DI_MISC_EXPORT')
    projMisc.appendIncludeDir('./pugixml')
    projMisc.pchHead = 'MiscPch.h'
    projMisc.pchSrc = 'MiscPch.cpp'

    projGfx = sln.addProject('DiGfx','/demi/gfx','DynamicLibrary')
    projGfx.setExtraPreDefines('DI_GFX_EXPORT')
    projGfx.appendIncludeDir('./addon')
    projGfx.appendIncludeDir('../misc')
    projGfx.appendIncludeDir(dxSDKIncDir)
    projGfx.appendDebugLibDir(dxSDKLibDir)
    projGfx.appendReleaseLibDir(dxSDKLibDir)
    projGfx.appendDependsLibDbg('DiMisc_d.lib;d3d9.lib;shlwapi.lib;dbghelp.lib;version.lib;')
    projGfx.appendDependsLibRel('DiMisc.lib;d3d9.lib;shlwapi.lib;dbghelp.lib;version.lib;')
    projGfx.pchHead = 'GfxPch.h'
    projGfx.pchSrc = 'GfxPch.cpp'
    projGfx.dependsProjs.append(projMisc)

    # projGUI = sln.addProject('DiGUI','/demi/gui','DynamicLibrary')
    # projGUI.setExtraPreDefines('DI_GUI_EXPORT;MYGUI_BUILD;MYGUI_USE_FREETYPE')
    # projGUI.appendIncludeDir('./D3D9Platform')
    # projGUI.appendIncludeDir('../misc')
    # projGUI.appendIncludeDir('../gfx')
    # projGUI.appendIncludeDir('../drvD3D9')
    # projGUI.appendIncludeDir(externalIncDir)
    # projGUI.appendIncludeDir(dxSDKIncDir)
    # projGUI.appendDebugLibDir(dxSDKLibDir)
    # projGUI.appendDebugLibDir(externalLibDbgDir)
    # projGUI.appendReleaseLibDir(dxSDKLibDir)
    # projGUI.appendReleaseLibDir(externalLibRelDir)
    # projGUI.appendDependsLibDbg('DiMisc_d.lib;DiGfx_d.lib;DiDrvD3D9_d.lib;d3d9.lib;d3dx9.lib;freetype2311_D.lib;')
    # projGUI.appendDependsLibRel('DiMisc.lib;DiGfx.lib;DiDrvD3D9.lib;d3d9.lib;d3dx9.lib;freetype2311.lib;')
    # projGUI.pchHead = 'MyGUI_Precompiled.h'
    # projGUI.pchSrc = 'MyGUI_Precompiled.cpp'

    projDrvD3D9 = sln.addProject('DiDrvD3D9','/demi/drvD3D9','DynamicLibrary')
    projDrvD3D9.setExtraPreDefines('DI_D3D9_EXPORT')
    projDrvD3D9.appendIncludeDir('../misc')
    projDrvD3D9.appendIncludeDir('../gfx')
    projDrvD3D9.appendIncludeDir(dxSDKIncDir)
    projDrvD3D9.appendDebugLibDir(dxSDKLibDir)
    projDrvD3D9.appendReleaseLibDir(dxSDKLibDir)
    projDrvD3D9.appendDependsLibDbg('DiMisc_d.lib;dxguid.lib;DiGfx_d.lib;d3d9.lib;d3dx9.lib;DxErr.lib;shlwapi.lib;dbghelp.lib;version.lib;')
    projDrvD3D9.appendDependsLibRel('DiMisc.lib;dxguid.lib;DiGfx.lib;d3d9.lib;d3dx9.lib;DxErr.lib;shlwapi.lib;dbghelp.lib;version.lib;')
    projDrvD3D9.pchHead = 'DrvD3D9Pch.h'
    projDrvD3D9.pchSrc = 'DrvD3D9Pch.cpp'
    projDrvD3D9.dependsProjs.extend([projMisc,projGfx])

    projExt = sln.addProject('DiExt','/demi/ext','DynamicLibrary')
    projExt.setExtraPreDefines('DI_EXT_EXPORT')
    projExt.appendIncludeDir('../misc')
    projExt.appendIncludeDir('../gfx')
    projExt.appendIncludeDir('../gui')
    projExt.appendIncludeDir(os.path.normpath(rootDir + '/external/src/ois/includes'))
    projExt.appendDebugLibDir(externalLibDbgDir)
    projExt.appendReleaseLibDir(externalLibRelDir)
    projExt.appendDependsLibDbg('DiMisc_d.lib;DiGfx_d.lib;OIS_d.lib;')
    projExt.appendDependsLibRel('DiMisc.lib;DiGfx.lib;OIS.lib;')
    projExt.pchHead = 'ExtPch.h'
    projExt.pchSrc = 'ExtPch.cpp'
    projExt.dependsProjs.extend([projMisc,projGfx])

    coreProjs = [projMisc,projGfx,projExt,projDrvD3D9]
    ####################################################################################################################
    # Samples
    addAppProj('Sample_Lighting','/Samples/Lighting',coreProjs)
    addAppProj('Sample_Car','/Samples/Car',coreProjs)
    addAppProj('Sample_EnvMap','/Samples/EnvironmentMap',coreProjs)
    addAppProj('Sample_Fresnel','/Samples/FresnelMaterial',coreProjs)
    addAppProj('Sample_Lava','/Samples/Lava',coreProjs)
    addAppProj('Sample_Hierarchy','/Samples/Hierarchy',coreProjs)
    #addAppProj('Sample_SSAO','/Samples/SSAO',coreProjs)

    ####################################################################################################################
    # Tests
    addAppProj('Test_BasicScene','/Tests/Test_BasicScene',coreProjs)
    addAppProj('Test_GUI','/Tests/Test_GUI',coreProjs)

    ####################################################################################################################
    # Tools
    # ogcvt = addAppProj('Tool_OgreConverter','/Tools/OgreConverter',coreProjs)
    # ogcvt.appendIncludeDir(os.path.normpath(externalIncDir + "/ogremain"))
    # ogcvt.appendDependsLibDbg('OgreMain_d.lib')
    # ogcvt.appendDependsLibRel('OgreMain.lib')
    # ogcvt.appendDebugLibDir(externalLibDbgDir)
    # ogcvt.appendReleaseLibDir(externalLibRelDir)

    # generate them
    sln.generateSln()
    sln.generateProjs()

if clean == True:
    cleanup()

if generateProjs == True:
    generate()

if generateExternal == True:
    generateExternalProjects()
