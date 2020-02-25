# hashlog
> Count similar lines in large log files

This is a small utility that rapidly reads large log file(s) in a single
pass, canonicalizes each line into a signature, counts how many times
each unique signature occurs, and prints the counts and signatures.  It
has low CPU, memory and I/O usage.

The amount of output should be small enough that strange log lines stand
ut, making it easier to find an anomaly among gigabytes of data. Other
programs such as `grep` can then be used to narrow in on lines in the
original log files.

## Theory

Each line of the log file is read and canonicalized into a signature.
This is done by replacing numeric values with '#'. Other strings are
also replaced by '#', such as the abbreviated names of months and
weekdays.

The elide() function determines which strings are elided in this
manner. The elide() function shows an example of how to elide two-digit
hex numbers such as might appear in MAC addresses.

When this works effectively, the number of unique signatures depends on
the number of unique printf's that applications are using to write into
the log file, rather than the number of log entries. That is typically
only in the hundreds.

Each signature is then hashed and added to a hash table that keeps track
of the unique count of occurrences for each signature. The size of the
hash table is proportional to the number of unique signatures and
usually quite small.

The hash table is then dumped, showing each signature prefixed by its
occurrence count.

## Build

The utility is an experiment rather than a complete package. It may be
desirable to customize it for unusual log formats.

This has been tested on Linux (Ubuntu) only, but is very generic code.

```sh
make
./test
```

## Usage examples

```sh
./hashlog /var/log/syslog | sort -n
./hashlog /var/log/*.log > /tmp/all.out
dmesg | ./hashlog -
```

## Release History

This is an experiment that has not been released.

## License

(c) 2020 Curt McDowell – coder#fishlet,com

Use of this code is unrestricted. USE AT OWN RISK.

[https://github.com/curtmcd/hashlog](https://github.com/curtmcd/hashlog)
