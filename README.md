Simple example of hot reloading in c

[![Example Image](images/reload.png)](https://example.com)

## Command to watch over file
```bash
while true; do
    make -C build/src/plugin -W src/plugin/plugin.c
    sleep 1
done
```
