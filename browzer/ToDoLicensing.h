<ul>
<li>Copy Protection Requirements
  <ul>
  <li> allow restricted/demo mode
  <li> unattended/online unlock
  <li> allow x # of copies to be installed per purchase
  <li> doesnt get in the way of the user
  </ul>

<li>Design
  <ul>
  <li> Upon first run, stamp in registry
  <li> if registry stamp detected then its not first run
  <li> put install date in stamp
  <li> put current date in stamp each run
  <li> check for back dating
  <li> encrypt stamp data
  <li> allow x # days of trial/demo run
  <li> implement via api so other copy protection libraries could be dropped in
  <li> get users email address etc. via a user registration dialog
  <li> also get a unique identifier and a random number
  <li> morph all that into a crypted string and post it onto my server
  <li> server sends back the unlock code
  <li> user pastes the code into registration dialog
  <li> registered code is appended to stamp in file and registry
  <li> only give unlock code if theyre paid up
  <li> need to find site to host the download/purchase
  <li> download from many different sites
  <li> purchase/unlock from one specific site
  <li> initially, allow download, send a check for payment and Ill email back the
    unlock code.
	</ul>

<li>R Run Time Protection:
	<ul>
   <li> Eval mode unless unlock code has been entered into registry
    <li>User registers with email address, etc.
    <li>Email unlock code to email address
    <li>Run crypt like function on data
	</ul>

<li>Unlock
	<ul>
    <li>Get a (atleast semi-) unique identifier off the machine.
    <li>Have em fill out a registration form
    <li>They click to send it to my server
    <li>I crunch up the data and email them back an unlock code.
    <li>They enter it and unlocks the app from demo mode.
    <li>The data must be stored in the registry, or somewhere

</ul>
<li>Demo mode
	<ul>
    <li>Possibilities
		<ul>
        <li>Limit # of files that can be added into library
        <li>Limit duration of song that can be played
        <li>Limit useage to a # of days
        <li>Limit # of songs allowed to be queued into Playlist
        <li>*** Restricted mode = cant add any more songs!

        <li>Full functionality eval for x days, then go to restricted mode until unlocked.
        <li>Full functionality eval for x days, then go to disabled mode until unlocked.

		<li>Full functionality eval except it only plays 3 songs then stops. Play resumes
		all they have to do is hit the play button.
		</ul>

</ul>
<li>Api:
	<ul>
    <li>MBLicense::MBLicense
		<ul>
        <li>public:
        <li>IsEval
        <li>IsRegistered
        <li>StoreRegInfo

        <li>private
        
        <li>GetRegInfo
        <li>ValidateRegKey
        <li>StoreRegKey
		</ul>
		</ul>

<li>Get a PO Box for MuzikBrowzer
		</ul>


