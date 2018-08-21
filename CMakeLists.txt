cmake_minimum_required( VERSION 3.7.2 )

# Set project information
project( SensorRack VERSION 0.0.1 LANGUAGES C )
set( PROJECT_BRIEF "A sensor system on RaspberryPi." )

# Options
set( CMAKE_C_FLAGS "-v -O2 -Wall" )
include_directories( /usr/local/include )
link_directories( /usr/local/lib )
add_definitions( -lrt -lwiringPi -Wl,-Map=board.map )

# Targets.
file( GLOB h_app ./app/if_lcd/ ./app/if_pc/ ./app/log/ ./app/menu/ )
file( GLOB h_hal ./hal/ ./hal/bme280/ ./hal/tsl2561/ )
file( GLOB h_sys ./sys/ )
include_directories( ${h_app} ${h_hal} ${h_sys} )

file( GLOB c_app  ./app/if_lcd/*.c ./app/if_pc/*.c ./app/log/*.c ./app/menu/*.c )
file( GLOB c_hal  ./hal/*.c ./hal/bme280/*.c ./hal/tsl2561/*.c )
file( GLOB c_sys  ./sys/*.c )
file( GLOB c_main ./main.c )

# Build and Link
add_executable( board.out ${c_app} ${c_hal} ${c_sys} ${c_main} )
target_link_libraries( board.out wiringPi )

