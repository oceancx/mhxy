add_executable(mhxy
	Astar.cpp Actor.cpp  GameMap.cpp Config.cpp MAP.cpp Sprite2.cpp WDF.cpp WAS.cpp  game.cpp program.cpp resource_manager.cpp shader.cpp sprite_renderer.cpp texture.cpp 
)

target_link_libraries(mhxy
	${ALL_LIBS}
)

#file(COPY config.txt sprite.frag sprite.vs X.png x.ttt
#     DESTINATION ${PROJECT_BINARY_DIR}/bin)
#configure_file(config.txt ${PROJECT_BINARY_DIR}/bin COPYONLY)
#configure_file(sprite.frag ${PROJECT_BINARY_DIR}/bin COPYONLY)
#configure_file(sprite.vs ${PROJECT_BINARY_DIR}/bin COPYONLY)
#configure_file(X.png ${PROJECT_BINARY_DIR}/bin COPYONLY)
#configure_file(x.ttt ${PROJECT_BINARY_DIR}/bin COPYONLY)

set_target_properties(mhxy PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

INSTALL(FILES config.txt sprite.frag sprite.vs X.png x.ttt DESTINATION ${CMAKE_INSTALL_PREFIX})
INSTALL(TARGETS mhxy RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})



