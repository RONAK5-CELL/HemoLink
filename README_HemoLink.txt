HEMOLINK - MODULARIZED PROJECT

WHAT CHANGED
- Split the large MainWindow implementation into Dashboard.cpp, DonorPage.cpp and InfoPages.cpp.
- Split donor console, donor GUI and donor maintenance functions into separate .cpp files.
- Split hospital maintenance and request maintenance functions into separate .cpp files.
- Corrected HospitalManager.h (it previously declared DonorManager by mistake).
- Corrected the extra character after the QMessageBox include.
- Added hospitals.txt because the hospital backend reads that exact filename.
- Added an empty requests.txt for new requests.
- Updated CMakeLists.txt to include all source files and copy data files to the build directory.

BUILD
1. Extract this folder.
2. Open CMakeLists.txt in Qt Creator.
3. Select the same Qt 6 MinGW kit used by your current project.
4. Configure Project / Run CMake.
5. Build HemoLink_GUI and run it.

IMPORTANT
- gui_main.cpp is the GUI application's entry point. main.cpp is the separate console entry point and is intentionally not included in the GUI target.
- The project was reorganized without intentionally changing the donor, hospital, inventory or request business logic.
- A Qt 6 MinGW toolchain is not available in this packaging environment, so a Windows Qt build could not be run here. Please build in Qt Creator and share the first compiler error, if any.
- Data files are copied to the build directory at CMake configure time. Back up edited data files before reconfiguring if you want to preserve test records.
