#!/usr/bin/env perl
use warnings;
use strict;
use Fcntl qw(LOCK_EX LOCK_UN);
use autodie qw(:io);
use File::Basename qw(dirname basename);
use File::Path qw(make_path);

if (scalar @ARGV < 1) {
    die("not enough arguments");
}
our $cdhist_file = "$ENV{HOME}/.cdhist";
my $search = $ARGV[0];
if ($search =~ m{^/(.+)/$}) {
    $search = qr{$1};
}
my $result = cdhist_search($cdhist_file, $search);
exit(1) if !defined $result;
print("$result\n");

sub cdhist_search {
    my ($filename, $search) = @_;
    my $lock_filename = "${filename}.lock";
    make_path(dirname($lock_filename));
    my $lock_fh = IO::File->new($lock_filename, 'a+') or die("$lock_filename: $!");
    flock($lock_fh, LOCK_EX);
    my $result;
    my $fh = IO::File->new($filename, 'r') or die("$filename: $!");
    local $_;
    while (<$fh>) {
        s{\R\z}{};
        s{^\s*\S+\s+\d+-\d+-\d+T\d+:\d+:\d+[-+]\d+\s*}{}; # skip the timestamp
        my $basename = basename($_);
        if (ref $search eq 'Regexp') {
            $result = $_ if $basename =~ $search;
        } else {
            $result = $_ if index($basename, $search) != -1;
        }
    }
    flock($lock_fh, LOCK_UN);
    close($lock_fh);
    return $result;
}
