Attention DOM maintainers!

It's very important to understand that most of these project files are
auto-generated via some Python vcproj conversion programs. Typically you'll work
on and edit one set of project files, then before you check in you run the
Python programs to auto-generate the rest of the project files based on the
updated projects you were working on.

There are two Python vcproj conversion programs: vc8to9.py and
changeColladaVersion.py. vc8to9.py converts Visual Studio 2005 (vc8) project and
solution files to Visual Studio 2008 (vc9). changeColladaVersion.py converts a
set of vcproj files to use a different version of Collada.

Currently we provide four different sets of project/solution files: VC8 and
Collada 1.4 (vc8-1.4), VC8 and Collada 1.5 (vc8-1.5), VC9 and Collada 1.4
(vc9-1.4), and VC9 and Collada 1.5 (vc9-1.5). Typically you'll work on the vc8
projects (either Collada 1.4 or 1.5) and then before you check in you'll want to
update all the other projects. Here's how you'd do that. Let's say we're working
on the VC8 files and Collada 1.4.

First, convert vc8-1.4 to vc8-1.5 by running changeColladaVersion.py:
python changeColladaVersion.py vc8-1.4 vc8-1.5 1.4 1.5

Next, convert vc8-1.4 to vc9-1.4 and vc8-1.5 to vc9-1.5:
python vc8to9.py vc8-1.4 vc9-1.4 convertVC8Tags
python vc8to9.py vc8-1.5 vc9-1.5 convertVC8Tags

Now all the projects are updated.
