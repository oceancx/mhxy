find_library(OpenGL_LIBRARY OpenGL)
find_library(Cocoa_LIBRARY Cocoa)
find_library(IOKit_LIBRARY IOKit)
find_library(CoreVideo_LIBRARY CoreVideo)

set(ALL_LIBS
	${Cocoa_LIBRARY} 
	${OpenGL_LIBRARY} 
	${IOKit_LIBRARY} 
	${CoreVideo_LIBRARY}
	glfw3
	GLEW_1130
	SOIL
)

link_directories(/Users/oceancx/CommonLib/lib/)

include_directories(
	/Users/oceancx/CommonLib/include/
	../include/
	.
)

add_executable(mhxy
	Astar.cpp Actor.cpp  GameMap.cpp Config.cpp MAP.cpp Sprite2.cpp WDF.cpp WAS.cpp  game.cpp program.cpp resource_manager.cpp shader.cpp sprite_renderer.cpp texture.cpp
)
target_link_libraries(mhxy
	${ALL_LIBS}
)

INSTALL(FILES config.txt sprite.frag sprite.vs X.png x.ttt DESTINATION ${CMAKE_INSTALL_PREFIX})
INSTALL(TARGETS mhxy RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})



