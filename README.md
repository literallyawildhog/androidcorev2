# androidcorev2

Roblox android executor both for 32bits and 64bits on 2.641.737. Just change the ARM64 to ARM to get the 32bit shared library, This is the worse code i've ever made so i wouldn't recommend using this or so. Please if you have any concerns or i wrote the code wrong please just correct it. Thankyou! and yeah, execution might not work idk.

# issues
* namespace globals::environment broke
* IDK WHY THIS SHIT BROKE

**Only has basic custom lua functions, but i will add more if i have time. Actively working on it until i got bored**

PLEASE CORRECT ME IF IM WRONG DISCORD : _nath.zi

To load androidcore into Roblox just compile the source, then get the library ``libandroidcore.so`` and put it into Roblox ``lib/`` folder. Now decompile **classes.dex** and go to ``com/roblox/client/startup/ActitySplash``. Once you are in there find the ``onCreate`` method and paste this below. TAKE NOTE! YOU NEED TO BYPASS PAIRIP TO MAKE THESE CHANGES.

```
const-string v0, "androidcore"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```
