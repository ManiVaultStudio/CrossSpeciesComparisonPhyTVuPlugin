from conans import ConanFile
from conan.tools.cmake import CMakeDeps, CMake, CMakeToolchain
from conans.tools import save, load
import os
import pathlib
import subprocess
from rules_support import PluginBranchInfo


class CrossSpeciesComparisonPhyTVuPluginConan(ConanFile):
    """Class to package using conan

    Packages both RELEASE and RELWITHDEBINFO.
    Uses rules_support (github.com/ManiVaultStudio/rulessupport) to derive
    versioninfo based on the branch naming convention
    as described in https://github.com/ManiVaultStudio/core/wiki/Branch-naming-rules
    """

    name = "CrossSpeciesComparisonPhyTVuPlugin"
    description = """Viewer of cell CrossSpeciesComparisonTreeData and tree metadata data as described in a .swc file."""
    topics = ("manivault", "plugin", "view", "CrossSpeciesComparisonPhyTVuPlugin")
    url = "https://github.com/ManiVaultStudio/CrossSpeciesComparisonPhyTVuPlugin"
    author = "julianthijssen@gmail.com"  # conan recipe author
    license = "LGPL 3.0"

    short_paths = True
    generators = "CMakeDeps"

    # Options may need to change depending on the packaged library
    settings = {"os": None, "build_type": None, "compiler": None, "arch": None}
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}

    # Data plugin dependencies
    requires = (
    "CrossSpeciesComparisonTreeData/cytosploreviewer@lkeb/stable",
    "CrossSpeciesComparisonTreeMetaData/cytosploreviewer@lkeb/stable",
    )

    # Qt requirement is inherited from hdps-core

    scm = {
        "type": "git",
        "subfolder": "hdps/CrossSpeciesComparisonPhyTVuPlugin",
        "url": "auto",
        "revision": "auto",
    }

    def __get_git_path(self):
        path = load(
            pathlib.Path(pathlib.Path(__file__).parent.resolve(), "__gitpath.txt")
        )
        print(f"git info from {path}")
        return path

    def export(self):
        print("In export")
        # save the original source path to the directory used to build the package
        save(
            pathlib.Path(self.export_folder, "__gitpath.txt"),
            str(pathlib.Path(__file__).parent.resolve()),
        )

    def set_version(self):
        # Assign a version from the branch name
        branch_info = PluginBranchInfo(self.recipe_folder)
        self.version = branch_info.version
        # print(f"Got version: {self.version}")

    def requirements(self):
        branch_info = PluginBranchInfo(self.__get_git_path())
        print(f"Core requirement {branch_info.core_requirement}")
        self.requires(branch_info.core_requirement)

    def configure(self):
        pass

    def system_requirements(self):
        #  May be needed for macOS or Linux
        pass

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def generate(self):
        generator = None
        if self.settings.os == "Macos":
            generator = "Xcode"
        if self.settings.os == "Linux":
            generator = "Ninja Multi-Config"

        tc = CMakeToolchain(self, generator=generator)

        tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"

        # Use the Qt provided .cmake files
        qt_path = pathlib.Path(self.deps_cpp_info["qt"].rootpath)
        qt_cfg = list(qt_path.glob("**/Qt6Config.cmake"))[0]
        qt_dir = qt_cfg.parents[0].as_posix()

        tc.variables["Qt6_DIR"] = qt_dir

        # Use the ManiVault .cmake file to find ManiVault with find_package
        mv_core_root = self.deps_cpp_info["hdps-core"].rootpath
        manivault_dir = pathlib.Path(mv_core_root, "cmake", "mv").as_posix()
        print("ManiVault_DIR: ", manivault_dir)
        tc.variables["ManiVault_DIR"] = manivault_dir
        
        # Give the installation directory to CMake
        MV_CSCTD_PATH = pathlib.Path(self.deps_cpp_info["CrossSpeciesComparisonTreeData"].rootpath).as_posix()
        tc.variables["MV_CSCTD_INSTALL_DIR"] = MV_CSCTD_PATH
        MV_CSCTMD_PATH = pathlib.Path(self.deps_cpp_info["CrossSpeciesComparisonTreeMetaData"].rootpath).as_posix()
        tc.variables["MV_CSCTMD_INSTALL_DIR"] = MV_CSCTMD_PATH

        # Set some build options
        tc.variables["MV_UNITY_BUILD"] = "ON"

        tc.generate()

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder="hdps/CrossSpeciesComparisonPhyTVuPlugin")
        cmake.verbose = True
        return cmake

    def build(self):
        print("Build OS is: ", self.settings.os)

        cmake = self._configure_cmake()
        cmake.build(build_type="RelWithDebInfo")
        cmake.build(build_type="Release")

    def package(self):
        package_dir = pathlib.Path(self.build_folder, "package")
        relWithDebInfo_dir = package_dir / "RelWithDebInfo"
        release_dir = package_dir / "Release"
        print("Packaging install dir: ", package_dir)
        subprocess.run(
            [
                "cmake",
                "--install",
                self.build_folder,
                "--config",
                "RelWithDebInfo",
                "--prefix",
                relWithDebInfo_dir,
            ]
        )
        subprocess.run(
            [
                "cmake",
                "--install",
                self.build_folder,
                "--config",
                "Release",
                "--prefix",
                release_dir,
            ]
        )
        self.copy(pattern="*", src=package_dir)

    def package_info(self):
        self.cpp_info.relwithdebinfo.libdirs = ["RelWithDebInfo/lib"]
        self.cpp_info.relwithdebinfo.bindirs = ["RelWithDebInfo/Plugins", "RelWithDebInfo"]
        self.cpp_info.relwithdebinfo.includedirs = ["RelWithDebInfo/include", "RelWithDebInfo"]
        self.cpp_info.release.libdirs = ["Release/lib"]
        self.cpp_info.release.bindirs = ["Release/Plugins", "Release"]
        self.cpp_info.release.includedirs = ["Release/include", "Release"]
