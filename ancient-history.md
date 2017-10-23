How We Got Here
===============

In 18 easy steps:

    $ mkdir ResultsViewer-Split
    $ cd ResultsViewer-Split
    $ git init --bare
    $ cd ../OpenStudio
    $ git subtree split --prefix=openstudiocore/src/resultsviewer -b split
    $ git push ../ResultsViewer-Split split:master
    $ cd ..
    $ mkdir ResultsViewer
    $ cd ResultsViewer
    $ git init
    $ vi ancient-history.md
    $ git add ancient-history.md
    $ git commit -m "How we got here..."
    $ git remote add -f ancient-history ../ResultsViewer-Split
    $ git merge -s ours --no-commit --allow-unrelated-histories ancient-history/master
    $ git read-tree --prefix=src/ -u ancient-history/master
    $ git commit -m "Initial code commit"
    $ git remote rm ancient-history
    
