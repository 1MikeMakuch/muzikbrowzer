#ifndef _my_help_msg_
#define _my_help_msg_
static CString helpmsg(){
	CString msg;

msg += "To change tag info on a bunch of mp3 files...\r\n";
msg += "\r\n";
msg += "1) In the \"mp3 files to edit\" box, click the Add button  as many times\r\n";
msg += "as needed to fill the files list box with  the desired mp3 files.\r\n";
msg += "\r\n";
msg += "2) In the \"Enter tag editing commands\" box you enter tag modification\r\n";
msg += "commands. \r\n";
msg += "\r\n";
msg += "To enter a Set command to be performed on all the above mp3 files, leave\r\n";
msg += "the Condition blank. In the Set group,  select the field to be modified\r\n";
msg += "and enter the desired value in the To New Value field.\r\n";
msg += "\r\n";
msg += "To enter a conditional Set command fill in the Condition by selecting the\r\n";
msg += "If Field, Operation and optionally  Value fields. The \"is\r\n";
msg += "blank\" and \"is not blank\" operations require no Value.\r\n";
msg += "\r\n";
msg += "The entered tag editing commands will be applied to each mp3 file selected\r\n";
msg += "in step 1. Each Set command will be  performed on each mp3 file if i)\r\n";
msg += "there is no Condition or ii) if the Condition evaluates true for that mp3\r\n";
msg += "file.\r\n";
msg += "\r\n";
msg += "3) Once you've entered the desired mp3 files (1) and  commands (2) you're\r\n";
msg += "ready to click the Apply button  to see what will happen to your mp3\r\n";
msg += "files. The Apply button will give you a report of what will happen (but\r\n";
msg += " won't do it yet).\r\n";
msg += "\r\n";
msg += "4) If the Apply report meets your expectation then click \"Doit\". If you \r\n";
msg += "click \"Cancel\" the edits won't be performed.\r\n";
msg += "\r\n";
msg += "Notes: You can edit an existing tag editing command by selecting it and\r\n";
msg += "clicking the Remove button. This will remove it and place it in the Add\r\n";
msg += "section where you can edit it and re-Add it.\r\n";
msg += "\r\n";
msg += "You can edit your mp3 files list and commands list all you want, do several\r\n";
msg += "Checks before finally doing Doit.\r\n";
msg += "\r\n";
msg += "MACROS:\r\n";
msg += "a) You can copy data from one tag field to another, by typing a field identifier\r\n";
msg += "in the Set To Value box.\r\n";
msg += "\r\n";
msg += "For example you want to set the Artists to \"Various Artists\" and you'd like\r\n";
msg += "the song title to read as \"Song Title - Artists\". To accomplish this do this;\r\n";
msg += "\r\n";
msg += "In the \"Set To Value\" field enter\r\n";
msg += "\r\n";
msg += "	\"{Title} - {Artists}\"\r\n";
msg += "\r\n";
msg += "And add another command that sets the Artist to \"Various Artists\"\r\n";
msg += "\r\n";
msg += "b) Convert to \"Lastname, Firstname\". To do this just enter\r\n";
msg += "\r\n";
msg += "	\"{LastName, FirstName}\" in the \"Set To Value\" box, and of course\r\n";
msg += "	select Artist as the field to modify.\r\n";
msg += "\r\n";
msg += "	You'll find this setting in the drop down list of pre defined values\r\n";
msg += "	as well as one to set back to \"{FirstName LastName}\".\r\n";
msg += "\r\n";
msg += "Renaming and/or moving files into ...\\Artist\\Album\\Track-Title.mp3 format.\r\n";
msg += "\r\n";
msg += "1 Add desired files into list box at top\r\n";
msg += "2 A guess is made at the default root directory. Modify it if you need to\r\n";
msg += "3 Click Add to add the Rename command into the Commands box\r\n";
msg += "4 Click Apply\r\n";
msg += "\r\n";
msg += "Note that you cannot mix Rename with other tag field modification operations,\r\n";
msg += "not as if you'd want to anyway.\r\n";

return msg;
}
#endif