from distutils.core import setup
import py2exe  
includes = ["encodings", "encodings.*", "os", "uuid", "sys"]  
options = {"py2exe":
            {"compressed": 1,
             "optimize": 2,
             "ascii": 1,
             "includes":includes,
             "bundle_files": 1
            }}
setup(
    options=options,
    zipfile=None,
    console=[{"script": "builder.py"}],  
    version = "beta",
    description = "Demi3D project builder",
    name = "Demi3D project builder",
)
