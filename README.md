# The spire

project c 2022

## Installation

### Windows 10/11 :

-   To compile this project on windows, you need to install mingw64 before. For this you can check https://www.msys2.org/
-   Once this is done, you can call make at the root of the project.
-   Just call the compiled file at build/main.out

### WSL :

-   If you are on wsl, you may need to install some packages before :

        sudo apt-get install make
        sudo apt-get install mingw-w64-x86-64-dev
        sudo apt-get install gcc-mingw-w64-x86-64

-   Once this is done, you can call this inscruction at the root of the project.

          make OS=Windows_NT CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar

-   Finally call the compiled file at build/main.out

## Add your files

-   [ ] [Create](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#create-a-file) or [upload](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#upload-a-file) files
-   [ ] [Add files using the command line](https://docs.gitlab.com/ee/gitlab-basics/add-file.html#add-a-file-using-the-command-line) or push an existing Git repository with the following command:

```
cd existing_repo
git remote add origin https://gitlab.com/d8546/the-spire.git
git branch -M main
git push -uf origin main
```
