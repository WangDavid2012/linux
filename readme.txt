我的github地址：
https://github.com/WangDavid2012/cpp.git

//创建一个本地仓库，然后在github上创建一个，与远端关联
…or create a new repository on the command line
echo "# cpp" >> README.md
git init
git add README.md
git commit -m "first commit"
git remote add origin https://github.com/WangDavid2012/cpp.git
git push -u origin master

…or push an existing repository from the command line
git remote add origin https://github.com/WangDavid2012/cpp.git
git push -u origin master

