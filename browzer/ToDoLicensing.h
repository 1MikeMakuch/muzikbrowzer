Copy Protection Requirements

  - allow restricted/demo mode
  - unattended/online unlock
  - allow x # of copies to be installed per purchase
  - doesn't "get in the way" of the user

Design

  - Upon first run, stamp in registry
  - if registry stamp detected then it's not first run
  - put install date in stamp
  - put current date in stamp each run
  - check for back dating
  - encrypt stamp data
  - allow x # days of trial/demo run
  - implement via api so other copy protection libraries could be dropped in
  - get users email address etc. via a user registration dialog
  - also get a unique identifier and a random number
  - morph all that into a crypted string and post it onto my server
  - server sends back the unlock code
  - user pastes the code into registration dialog
  - "registered" code is appended to stamp in file and registry
  - only give unlock code if they're paid up
  - need to find site to host the download/purchase
  - download from many different sites
  - purchase/unlock from one specific site
  - initially, allow download, send a check for payment and I'll email back the
    unlock code.

-R Run Time Protection:
    Eval mode unless unlock code has been entered into registry
    User registers with email address, etc.
    Email unlock code to email address
    Run crypt like function on data

Unlock
    Get a (atleast semi-) unique identifier off the machine.
    Have 'em fill out a registration form
    They click to send it to my server
    I crunch up the data and email them back an unlock code.
    They enter it and unlocks the app from demo mode.
    The data must be stored in the registry, or somewhere

Demo mode
    Possibilities
        Limit # of files that can be added into library
        Limit duration of song that can be played
        Limit useage to a # of days
        Limit # of songs allowed to be queued into Playlist
        *** Restricted mode = can't add any more songs!

        Full functionality eval for x days, then go to restricted mode until unlocked.
        Full functionality eval for x days, then go to disabled mode until unlocked.

		Full functionality eval except it only plays 3 songs then stops. Play resumes
		all they have to do is hit the play button.

Api:
    MBLicense::MBLicense
        public:
        IsEval
        IsRegistered
        StoreRegInfo

        private
        
        GetRegInfo
        ValidateRegKey
        StoreRegKey

        


Get a PO Box for MuzikBrowzer

*/
