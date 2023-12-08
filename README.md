# NAME

slhtml - Simple/Suckless HTML processor

# SYNOPSIS

    slhtml SCRIPT < HTML > HTML

# DESCRIPTION

This is a simple HTML processor that uses a script to process HTML
tags. The script is executed with the tag name as the first argument
and the tag body as the standard input.

The script should be a valid executable file. It should accept
the following arguments:

    --list      : List the tags that the script can process.
    TAG ARGS... : Process the tag with the given arguments.

The script should output the processed HTML to the standard output.

# EXAMPLES

manpage.sl.html

    <html>
        <head>
        </head>
        <body>
    
            <h1>SLHTML example printing two manpages</h1>

            <p>Tag parameters are passed as arguments.</p>
            <MANPAGE1 cat/>
        
            <p>The body of the tag is passed to the standard input.</p>
            <MANPAGE2>head</MANPAGE2>
    
        </body>
    </html>


examples.sl.sh

    #!/bin/sh -e
    case "$1" in
        --list)
            echo "MANPAGE1 MANPAGE2";;
        MANPAGE1)
            echo "<pre><code>"
            man "$2" | cat
            echo "</code></pre>"
            ;;
        MANPAGE2)
            echo "<pre><code>"
            man $(cat) | cat
            echo "</code></pre>"
            ;;
        *)
            echo "$*" >&2
            ;;
    esac

GNUmakefile

    all: manpage.html

    %.html: %.sl.html examples.sl.sh
        slhtml ./examples.sl.sh < $< > $@

# RETURN VALUE

0 on success, 1 on failure, 2 on invalid input.

# COLLABORATING

For making bug reports, feature requests and donations visit
one of the following links:

1. [gemini://harkadev.com/oss/](gemini://harkadev.com/oss/)
2. [https://harkadev.com/oss/](https://harkadev.com/oss/)
# SEE ALSO

**SED(1)**
