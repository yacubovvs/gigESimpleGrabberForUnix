# gigESimpleGrabberForUnix
Simple grabber for Cognex, Basler and ither cameras via GigE

____

### English
Simple tool fot getting image from GigE Cameras as Cognex or Basler in terminal for unix systems.

To set up the camera use Pylon - https://www.baslerweb.com/en/products/software/basler-pylon-camera-software-suite/
Required Pylon 6+ library

Don't want to build project - https://github.com/yacubovvs/gigeSimpleGrabber/tree/master/bin/Release

Compile:
```
make
```

Usage:
```
Parameters:
	-s  Camera serial number
	-o  Path to output file
	-d  Inner package delay in ticks (default 50)
	-a  Attempts tо grab image (default 1)
	-p  Package size (default 1500)
	-e  Exposure time (default 35000)
	-t  Patch to sample file
```

The sample file in BMP format is required to get the desired headers and can be made using the Pylon utility

Example:
```
/opt/gigEGrabber/SimpleGrab -s 23506644 -d 10 -p 1500 -e 450000 -a 10 -o /opt/gigEGrabber/out_test.bmp -t /opt/gigEGrabber/image.bmp
```

Feel free to use it for commercial purposes.
Based on Pylon examples.

Tested on Debian 9 and Debian 10

____
### Русский
Простая утилита для получения изображения с камер с интерфейсовм GigE (таких как Basler или Cognex) через командную строку для unix систем.

Для настройки камер, используйте утилиту Pylon - https://www.baslerweb.com/ru/produkty/programmnoje-obespechenie/basler-pylon-camera-software-suite/
Для запуска приложения необхоима установленная утилита Pylon 6+ 

Компиляция:
```
make
```

Использование:
```
Параметры
  -s  Серийный номер камеры
	-o  Путь до нового файла
	-d  Задержка в передаче пакетов (по умолчанию 50)
	-a  Попыток получить изображение (по умолчанию 1)
	-p  Рамзер пакета (по умолчанию 1500)
	-e  Время экспонирования (по умолчанию 35000)
	-t  Путь до файла шаблона
```

Файл шаблона в формате BMP необходим для получения нужных заголовков и может быть сделан с помощью утилиты Pylon

Пример:
```
/opt/gigEGrabber/SimpleGrab -s 23506644 -d 10 -p 1500 -e 450000 -a 10 -o /opt/gigEGrabber/out_test.bmp -t /opt/gigEGrabber/image.bmp
```

Разрешено использование в коммерчиских целях.
Основано на примерах Pylon.

Тестирование проводилось на системах Debian 9 и Debian 10

____
