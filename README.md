# Plog
*Plog* is C logging library for Linux that is thread-safe, offers a way to be stripped away from compilation and allows the persistent configuration of log severity level, log file size, log file count, terminal mode and buffer size through *plog.conf* configuration file and through API.

You can download the latest released binary or (gcc and make are required to be installed):
- git clone git@github.com:stefanGaina/Plog.git
- cd *Plog*
- make

The *libplog.so* can be found in *plog/bin* and the header files that will need to be included in the application are in *plog/include* (without the ones in the internal directory).

# Severity level
The are 7 logging functions: **plog_fatal()**, **plog_error()**, **plog_warn()**, **plog_info()**, **plog_debug()**, **plog_trace()**, **plog_verbose()**.
The effects of these functions can be enabled/disabled at runtime through **plog_set_severity_level()** and **plog_get_severity_level()** or through the "LOG_LEVEL = " in *plog.conf*.
The value is a bit mask (more information can be found in *plog.h*).

# File size
The logs are stored in a file of choice. The maximum size of the file can be set at runtime through **plog_set_file_size()** and **plog_get_file_size()** or through the "FILE_SIZE = " in *plog.conf*.
The value is in bytes (more information can be found in *plog.h*).

# File count
If the maximum file size is reached instead of deleting the contents of the file a new file can be created. The count of these created file can be set at runtime through **plog_set_file_count()** and **plog_get_file_count()** or through "FILE_COUNT = " in *plog.conf*. More information can be found in *plog.h*.

# Terminal mode
The logs besides being stored in a file can also be printed in the terminal. This mode can be set at runtime through **plog_set_terminal_mode()** and **plog_get_terminal_mode()** or through the "TERMINAL_MODE = " in *plog.conf*. The benefits are that they can be seen live and can be easier to be read because they are colored. More information can be found in *plog.h*.

# Buffer size
While the logs in the terminal can ease debugging they have a huge performance impact on the application. To mitigate this Plog allows for the logs to be buffered and be printed asynchronically (the logs will still take some time to be printed but the application's thread is being unblocked faster, check *example* for performance test). The buffer size (in bytes) of each buffered log can be set at runtime through **plog_set_buffer_size()** and **plog_get_buffer_size()** or through the "BUFFER_SIZE = " in *plog.conf*. More information can be found in *plog.h*.

# Persistency
The previously mentioned features are persistent. They are being read from *plog.conf* (if the file does not exist one will be created with default values) during **plog_init()** and any changes done at runtime will be written in the same configuration file during **plog_deinit()**. This is why any function call before **plog_init()** is invalid and any function call after **plog_deinit()** is invalid.

# Compile time filter
Besides the runtime severity level filter the function calls can be stripped from the compilation. This can be done with *-DPLOG_STRIP_FATAL*, *-DPLOG_STRIP_ERROR*, *-DPLOG_STRIP_WARN*, *-DPLOG_STRIP_INFO*, *-DPLOG_STRIP_DEBUG*, *-DPLOG_STRIP_TRACE*, *-DPLOG_STRIP_VERBOSE*, or *-DPLOG_STRIP_ALL*.
