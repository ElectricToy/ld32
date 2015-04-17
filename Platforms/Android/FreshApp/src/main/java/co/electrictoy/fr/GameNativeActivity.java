package co.electrictoy.fr;

import co.electrictoy.fr.FreshNativeActivity;

import co.electrictoy.ld32.R;

public class GameNativeActivity extends FreshNativeActivity
{
	@Override
	protected String BASE64_PUBLIC_KEY()
	{
		// ** TODO ** Modify per game.
        return "";
	}

	@Override
	protected byte[] SALT()
	{
		// ** TODO ** Modify per game.
	    return new byte[] {
			
		};
	}

	@Override
	protected boolean enableLicenseChecking()
	{
		// TODO!!! Set to true at release.
		return false;
	}
	
	static
	{
		System.loadLibrary( "ld32" );
	}	
}
