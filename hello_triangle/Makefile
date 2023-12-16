APP_NAME = app 
BUILD_DIR = ./bin
C_FILES = ./src/*.c

APP_DEFINES:=
APP_INCLUDES:= -I ./include -framework Cocoa -framework OpenGL -framework IOKit
APP_LINKERS:= -L ./lib -lglfw3

build:
	clang ${C_FILES} -o ${BUILD_DIR}/${APP_NAME} ${APP_INCLUDES} ${APP_LINKERS}

