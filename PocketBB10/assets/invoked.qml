import bb.cascades 1.0
Page {
    
    Container {
        leftPadding: 20;
        rightPadding: 20;
        topPadding: 30;
        background: Color.create ("#F1E3AD")
        Label {
            text: "New Link Saved";
            horizontalAlignment: HorizontalAlignment.Center;
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontWeight: FontWeight.Bold
                fontSize: FontSize.XLarge
                color: Color.create("#A06C42")    
            }
        }
        Divider {
        }
        Label {
            text: "<html><body><center>Your link is added to your pocket list.</center><br /><br /><center>     Press the <b>Sync</b> button to update<br />          your list with the new link the <br/>            next time you open the app</center></body></html>";
            horizontalAlignment: HorizontalAlignment.Center;
            multiline: true
            textStyle { color: Color.create("#000000") }
        }
        ImageView {
            topMargin: 100;
            imageSource: "asset:///images/check.png";
            horizontalAlignment: HorizontalAlignment.Center;
        }
        Button {
            text: "OK";
            horizontalAlignment: HorizontalAlignment.Center;
            topMargin: 200;
            onClicked: {
                //Application.quit();
                app.closeMe();
            }
        }
    }
}
