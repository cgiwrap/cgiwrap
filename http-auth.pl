#!/umr/bin/perl

$| = 1;

#
# Usage:
#   call ProcessHTGroupFile and/or ProcessUnixGroupFile for each group
#   file you want to use
#
#   check user group membership using the UserInGroup(user,group) call
#
#   if auth failed, called AuthentHeader(0,"realm"), else AH(1);
#
#   You'll have to use NPH style scripts for the authentication to work
#   properly. (Otherwise, no way to re-authenticate if authentication fails)
#



sub AuthentHeader
{
        local ( $AUTHENTICATED, $REALM ) = @_;
        local ($server, $serverproto);

        $server = $ENV{"SERVER_SOFTWARE"};
        $serverproto = $ENV{"SERVER_PROTOCOL"};

        if ( $AUTHENTICATED )
        {
                print "$serverproto 200 Ok\n";
        }
        else
        {
                print "$serverproto 401 Authentication Failed\n";
                print "WWW-Authenticate: Basic realm=\"$REALM\"\n";
        }

        print "Server: $server\n";
}


sub ProcessHTGroupFile
{
	local ($GRP_FILE) = @_;

	$GROUP_DATA{"cgiauth:nneul"} = 1;
}


sub ProcessUnixGroupFile
{


}


sub UserInGroup
{
	local ($USER,$GROUP) = @_;

	if ( $GROUP_DATA{$GROUP . ":" . $USER} )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

1;
