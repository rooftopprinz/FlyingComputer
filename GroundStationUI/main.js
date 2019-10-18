const {app, BrowserWindow, Menu} = require('electron')

app.whenReady().then(function()
    {
        const mainWindow = new BrowserWindow({
            width: 800,
            height: 600,
            webPreferences: {
                nodeIntegration: true
            }
        })

        const menu = Menu.buildFromTemplate([
            {
                label: "&File"
            }
        ]);

        Menu.setApplicationMenu(menu);

        mainWindow.loadFile('frontend/index.html');
        mainWindow.webContents.openDevTools();
    });