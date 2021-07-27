## Implementation Notes

Most of application logic implemented in [WordCounterThread](src/models/word_counter_thread.h) class.

Given input `file_name`:

* opens file in separate thread
* reads file line by line converting them to lower case
* splits line into words, accordingly to the [Unicode text boundry specification](http://www.unicode.org/reports/tr29/)
* for each distinct word, stores it's count in word count histogram (described as `QHash<QString, quint32>`)

Progress, errors and results are reported by various signals of `WordCounterThread` class.

Access to the word count histogram protected by mutex, so main thread can peak intermediate results during file processing.

[WordCounter](src/models/word_counter.h) acts as frontend for `WordCounterThread`, hiding all thread managing logic from the caller:

* manages lifetime and execution of a `WordCounterThread` instance
* provides access to progress and status of file processing through properties

[WordCounterModel](src/models/word_counter_model.h) wraps instance of `WordCounter` and provides data for UI:

* connects to the status updates of `WordCounter`
* when file processing starts, initiates internal timer to peak intermediate results

## Build Requirements

* C++17 compliant compiler
* CMake 3.15 or newer
* Qt 5.15

## Building From Source

```shell
git clone https://github.com/ovfireandvoid/word_counter.git
cd word_counter
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Testing

```shell
git clone https://github.com/ovfireandvoid/word_counter.git
cd word_counter
mkdir build
cd build
cmake .. -DENABLE_TESTING=ON
ctest .
```
