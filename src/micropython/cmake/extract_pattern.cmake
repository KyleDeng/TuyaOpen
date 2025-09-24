# Extract pattern from source files
# This is a generic pattern extraction script used by mpy_prepare.cmake
# 
# Usage:
#   cmake -DSOURCE_FILES="file1.c;file2.c" \
#         -DOUTPUT_FILE=output.txt \
#         -DPATTERN="MP_REGISTER_MODULE" \
#         -P extract_pattern.cmake

# Parse the source files list
string(REPLACE ";" ";" SOURCE_FILE_LIST "${SOURCE_FILES}")

# Open output file
file(WRITE ${OUTPUT_FILE} "")

# Process each source file
foreach(SRC_FILE ${SOURCE_FILE_LIST})
    if(EXISTS ${SRC_FILE})
        # Read the file content
        file(READ ${SRC_FILE} FILE_CONTENT)
        
        # Extract lines containing the pattern
        # For MP_REGISTER_MODULE, we want the entire macro call
        if(PATTERN STREQUAL "MP_REGISTER_MODULE")
            # Match MP_REGISTER_MODULE(...) including multi-line
            string(REGEX MATCHALL "MP_REGISTER_MODULE\\([^)]*\\)" MATCHES "${FILE_CONTENT}")

            foreach(MATCH ${MATCHES})
                # Clean up the match (remove excessive whitespace/newlines)
                string(REGEX REPLACE "\n" " " CLEAN_MATCH "${MATCH}")
                string(REGEX REPLACE "[ \t]+" " " CLEAN_MATCH "${CLEAN_MATCH}")

                # Write to output file
                file(APPEND ${OUTPUT_FILE} "${CLEAN_MATCH}\n")

                # Debug output
                message(STATUS "Found: ${CLEAN_MATCH}")
            endforeach()
        elseif(PATTERN STREQUAL "MP_REGISTER_ROOT_POINTER")
            # Match MP_REGISTER_ROOT_POINTER(...) including multi-line
            string(REGEX MATCHALL "MP_REGISTER_ROOT_POINTER\\([^)]*\\)" MATCHES "${FILE_CONTENT}")

            foreach(MATCH ${MATCHES})
                # Clean up the match (remove excessive whitespace/newlines)
                string(REGEX REPLACE "\n" " " CLEAN_MATCH "${MATCH}")
                string(REGEX REPLACE "[ \t]+" " " CLEAN_MATCH "${CLEAN_MATCH}")

                # Write to output file
                file(APPEND ${OUTPUT_FILE} "${CLEAN_MATCH}\n")

                # Debug output
                message(STATUS "Found: ${CLEAN_MATCH}")
            endforeach()
        else()
            # Generic pattern matching (for other potential uses)
            string(REGEX MATCHALL ".*${PATTERN}.*" MATCHES "${FILE_CONTENT}")
            
            foreach(MATCH ${MATCHES})
                file(APPEND ${OUTPUT_FILE} "${MATCH}\n")
            endforeach()
        endif()
    endif()
endforeach()

message(STATUS "Pattern extraction complete: ${OUTPUT_FILE}")