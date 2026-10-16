# Helper function to add an example executable
function(add_ottunity_example name)
    add_executable(${name} ${ARGN})
    
    target_link_libraries(${name}
        PRIVATE
            ottunity
            unity::framework
    )

    if(BUILD_TESTING)
        add_test(
            NAME ottunity_${name}
            COMMAND $<TARGET_FILE:${name}>
        )
    endif()
endfunction()

# Define examples
add_ottunity_example(example_basic basic.c)
add_ottunity_example(example_groups groups.c)
add_ottunity_example(example_multi_file
    multi_file/main.c
    multi_file/module_a.c
    multi_file/module_b.c
)
add_ottunity_example(example_unity unity.c)
