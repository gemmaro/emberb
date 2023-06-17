# Locale::Po4a::Ruby -- Convert Ruby language files to PO file, for translation.
#
# This program is free software; you may redistribute it and/or modify
# it under the terms of GPL.
#

############################################################################
# Modules and declarations
############################################################################

use Locale::Po4a::TransTractor qw(process new);
use Locale::Po4a::Common;

package Locale::Po4a::Ruby;

use 5.006;
use strict;
use warnings;

require Exporter;

use vars qw(@ISA @EXPORT $AUTOLOAD);
@ISA    = qw(Locale::Po4a::TransTractor);
@EXPORT = qw();

my $debug = 0;

sub initialize { }

sub parse {
    my $self = shift;
    my ( $line, $ref );
    my $par;

  LINE:
    ( $line, $ref ) = $self->shiftline();

    while ( defined($line) ) {
        chomp($line);
        print STDERR "begin\n" if $debug;

        if ( $line =~ m/([^#]*#\s*)([^!].*)/ ) {
            my $pre     = $1;
            my $content = $2;

            my $t = $self->translate( $content, $ref, "comment" );
            $self->pushline( $pre . $t . "\n" );
        } else {
            $self->pushline( $line . "\n" );
        }

        # Reinit the loop
        ( $line, $ref ) = $self->shiftline();
    }
}

##############################################################################
# Module return value and documentation
##############################################################################

1;
__END__

=encoding UTF-8

=head1 NAME

Locale::Po4a::Ruby - convert Ruby language files from/to PO files

=head1 DESCRIPTION

Locale::Po4a::Ruby is a module to help the translation of Ruby
language files into other [human] languages.

The module searches for lines of the following format and extracts the
quoted text:

 # this is a comment

excluding shebang:

 #!/usr/bin/env ruby

=head1 SEE ALSO

L<Locale::Po4a::TransTractor(3pm)>, L<po4a(7)|po4a.7>

=head1 AUTHORS

 gemmaro <gemmaro.dev@gmail.com>

=head1 COPYRIGHT AND LICENSE

Copyright © 2023 gemmaro

This program is free software; you may redistribute it and/or modify it
under the terms of GPL.

=cut
