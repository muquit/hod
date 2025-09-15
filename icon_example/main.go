package main

//
// example of using the icon by converting a PNG
// with 'hog -d icon.png > assets/icon.go

import (
    "github.com/getlantern/systray"
	"github.com/muquit/hod/icon_example/assets"
)

func main() {
    systray.Run(onReady, onExit)
}

func onReady() {
    systray.SetIcon(Icon.Data)
    systray.SetTitle("hog -g example")
    systray.SetTooltip("hog -g example")
    mQuit := systray.AddMenuItem("Quit", "Quit the whole app")
    mQuit.SetIcon(Icon.Data)

    go func() {
        for {
            select {
            case <-mQuit.ClickedCh:
                systray.Quit()
                return
            }
        }
    }()
}

func onExit() {
    // Cleanup
}
