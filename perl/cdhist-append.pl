#!/usr/bin/env perl
use warnings;
use strict;
use IO::File;
use Fcntl qw(LOCK_EX LOCK_UN F_GETFL F_SETFL SEEK_SET);
use POSIX qw(strftime);
use autodie qw(:io);
use File::Path qw(make_path);
use File::Basename qw(dirname);

if (scalar @ARGV < 1) {
    die("not enough arguments\n");
}
our $cdhist_file = "$ENV{HOME}/.cdhist";
cdhist_append($cdhist_file, $ARGV[0]);

sub cdhist_append {
    my ($filename, $append_dirname) = @_;
    my $lock_filename = "${filename}.lock";
    make_path(dirname($lock_filename));
    my $lock_fh = IO::File->new($lock_filename, 'a+') or die("$lock_filename: $!");
    flock($lock_fh, LOCK_EX);

    make_path(dirname($filename));
    my $fh = IO::File->new($filename, 'a+') or die("$filename: $!");
    $fh = IO::File->new($filename, 'r+'); # would fail on nonexistent files

    # remove O_APPEND so we can write at any location
    my $flags = fcntl($fh, F_GETFL, 0);
    if ($flags & O_APPEND) {
        $flags &= ~O_APPEND;
        fcntl($fh, F_SETFL, $flags);
    }

    seek($fh, 0, SEEK_SET);
    my $in_sync = 1;
    my $read_pos = 0;
    my $write_pos = 0;
    while (<$fh>) {
        my $line = $_;
        s{^\s*\S+\s+\d+-\d+-\d+T\d+:\d+:\d+[-+]\d+\s*}{}; # timestamp
        $read_pos = tell($fh);
        my $dir = $_;
        $dir =~ s{\R\z}{};
        if ($dir ne $append_dirname) {
            if ($in_sync) {
                $write_pos = $read_pos;
                next;
            }
            seek($fh, $write_pos, SEEK_SET);
            print $fh $line;
            $write_pos = tell($fh);
            seek($fh, $read_pos, SEEK_SET);
            next;
        }
        $in_sync = 0;
    }
    seek($fh, $write_pos, SEEK_SET);
    my $timestamp = strftime("%a %Y-%m-%dT%H:%M:%S%z", localtime());
    printf $fh ("%s %s\n", $timestamp, $append_dirname);
    truncate($fh, tell($fh));
    close($fh);
    flock($lock_fh, LOCK_UN);
    close($lock_fh);
}

# autodie doesn't wrap tell.
sub tell {
    my $pos;
    if (($pos = CORE::tell(@_)) < 1) {
        die("tell: $!");
    }
    return $pos;
}
