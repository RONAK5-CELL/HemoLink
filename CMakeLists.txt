
cmake_minimum_required(VERSION 3.16)

project(HemoLink LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(HemoLink_GUI
    gui_main.cpp
    mainwindow.cpp
    mainwindow.h
    LiveEmergencyRadar.cpp
    AdminPanel.cpp

    Dashboard.cpp
    DonorPage.cpp
    InfoPages.cpp
    EmergencyRequests.cpp
    DonorMatching.cpp
    ReportsAnalytics.cpp
    DonationHistory.cpp
    NotificationCenter.cpp

    DonorManager.cpp
    DonorManagerMaintenance.cpp
    DonorManagerGUI.cpp
    Donormanager.h
    Donor.h

    HospitalManager.cpp
    HospitalManagerMaintenance.cpp
    HospitalManager.h

    Inventory.cpp
    Inventory.h

    RequestManager.cpp
    RequestManagerMaintenance.cpp
    RequestManager.h
    Request.h
)

target_link_libraries(HemoLink_GUI PRIVATE Qt6::Widgets)

# DATA FILES
set(DATA_FILES
    donors.txt
    hospitals.txt
    blood_stock.txt
    requests.txt
)

foreach(FILE ${DATA_FILES})

    set(SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}")
    set(DEST_FILE "${CMAKE_CURRENT_BINARY_DIR}/${FILE}")

    if(EXISTS "${SOURCE_FILE}")

        configure_file(
            "${SOURCE_FILE}"
            "${DEST_FILE}"
            COPYONLY
        )

    elseif(NOT EXISTS "${DEST_FILE}")

        file(WRITE "${DEST_FILE}" "")

    endif()

endforeach()
