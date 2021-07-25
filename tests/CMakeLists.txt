
find_package(Qt5Test REQUIRED)

# WordCounter Test

add_executable(word_counter.test word_counter.test.cpp)

add_test(NAME word_counter.test COMMAND word_counter.test)

target_link_libraries(word_counter.test PRIVATE Qt5::Test)

target_sources(word_counter.test PRIVATE
    ../src/models/word_counter.cpp
    ../src/models/word_counter_thread.cpp
)

# WordCounterModel Test

add_executable(word_counter_model.test word_counter_model.test.cpp)

add_test(NAME word_counter_model.test COMMAND word_counter_model.test)

target_link_libraries(word_counter_model.test PRIVATE Qt5::Test)

target_sources(word_counter_model.test PRIVATE
    ../src/models/word_counter.cpp
    ../src/models/word_counter_model.cpp
    ../src/models/word_counter_thread.cpp
)

# Source Groups

source_group(src REGULAR_EXPRESSION "^.*\.(cpp|h|qrc)$")
