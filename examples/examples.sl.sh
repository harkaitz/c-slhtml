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
