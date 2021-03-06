
include_directories(../src)

#List all cpp files - assume all of them really represent a test.
file(GLOB LUAScript_Test_Sources *.cpp)

#Each *.cpp will be a test-executable.
foreach(testSourceFile ${LUAScript_Test_Sources})
    
	#Name of the executable deduced from the filename
	get_filename_component(testName ${testSourceFile} NAME_WE)
	
	message(STATUS "Creating executable for test - '${testName}'")
	
	# Set up the test
	add_executable(${testName} ${testSourceFile})
	
	#Link to boost' unit-test framework
	target_link_libraries(${testName} ${Boost_LIBRARIES} LibBase LibScriptingService LibLuaScripting)
	
	add_test(NAME ${testName} COMMAND ${testName})	

endforeach(testSourceFile)

# Copy all testdata
file(GLOB LUA_TESTDATA_FILES "./testdata/*.*")
file(COPY ${LUA_TESTDATA_FILES} DESTINATION ${TESTDATA_DIRECTORY})