# SimplePDF

## Description
A very minimalist PDF file viewer


![](https://github.com/Nikki-M12/simple-pdf/blob/main/spdf.png)


## How to compile

### Step 1: Install Dependencies
### Ubuntu/Debian
```bash
sudo apt install git qtbase5-dev libpoppler-qt5-dev build-essential g++ make
```
### Fedora / RHEL / CentOS
```bash
sudo dnf install git qt5-qtbase-devel poppler-qt5-devel gcc-c++ make
```
```bash
sudo dnf install git qt5-qtbase-devel poppler-qt5-devel gcc-c++ make
```
```bash 
sudo yum install git qt5-qtbase-devel poppler-qt5-devel gcc-c++ make
```
### Arch Linux / Manjaro
```bash
sudo pacman -S git qt5-base poppler-qt5 base-devel gcc make
```
### OpenSUSE
```bash
sudo zypper install git libqt5-qtbase-devel poppler-qt5-devel gcc-c++ make
```
### Alpine Linux
```bash
sudo apk add qt5-qtbase-dev git poppler-qt5-dev g++ make
```


## Step 2: Compile the project

```bash
git clone https://github.com/Nikki-M12/simple-pdf && cd simple-pdf
```
```bash
qmake && make && make clean
```

To run the program you can do so with the command:

```bash
./SimplePDF
```
