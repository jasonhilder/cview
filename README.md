# cview
View github contributions in the terminal.

### Build instructions:

```
mkdir build && cd build && cmake ..
```

then

```
cmake --build .
```

### Currently dependencies are:

* libssl-dev (ubuntu)


Add a project or a directory of multiple projects with:
```
cv -a /path/to/directory/of/projects
```
or
```
cv -a /path/to/single/project
```


Example config file:
```
email:
user@email.com

exclude:
vendor
node_modules

projects (expected root as home folder):
/path/to/project/one
/path/to/project/two

```
