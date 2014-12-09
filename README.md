T-Space Checkout System
===================
This is a project that will allow T-Space members to checkout various tools and kits for use outside of the T-Space.

The system will be arduino-based and utilize a LCD, RFID reader, and ethernet shield.

The system will communicate back to a central server once an ID is scanned. There the UID read will be compared to a MySQL datatable of all the member's UID's. If the member is found and they are allowed to check out equipment, they will be prompted to scan the tool/kit they either wish to checkout or return.
 
If the tool's UID is found, the program will then check to see if the tool is checked out already and check it in if it was. The user will then receive a message thanking them for returning the tool. The entry will then be marked as returned under the member's UID. If the tool is already in, it will be checked out under the member's UID with a date stamp. The user will receieve a message telling them that the tool has been successfully checked out.
 
Additionally, the system will run a server-side program that checks the customizable checkout time limit to see if the members need to return items and will email them the due date of the item if it is nearing the day it is due. The system will also be set to auto-suspend the member's priviledge to checkout an item if they have not returned an item due. This program will also be set up to limit the number of items the members can have checked out.