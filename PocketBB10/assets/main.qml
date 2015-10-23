// Default empty project template
import bb.cascades 1.0

// creates one page with a label
Page {
    Container {
        layout: StackLayout {orientation: LayoutOrientation.TopToBottom}
        TextField {
            id: checkTxt
            objectName: checkText
            text: "Token"
        }
        Button {
            id: bottone
            text: "check starts with"
            onClicked:  {
                console.log("clicked" + checkTxt.text)
                app.askCode()
            }
        }
        Label {
          id: lbl
          objectName: "lbl"
          leftMargin: 40
          text: "_"
        }
        ScrollView {
        WebView {
                    id: myWebView
                    objectName: "myWebView"
                    url: ""
                    onUrlChanged: {
                        console.log("Changed " + url)
                    }
                    onLoadingChanged: {
                        if (loadRequest.status ==  WebLoadStatus.Started ) {
                          // lbl.setText
                          console.log("Load started." + loadRequest.url)
                        }
                        else if (loadRequest.status ==  WebLoadStatus.Succeeded ) {
                            console.log("+++from QML got: "+loadRequest.url);
                            var patt = /^https:\/\/getpocket.com\/auth\/pocketbb10auth/;
                            if(patt.test(loadRequest.url)) {
                               console.log("OAuthCode found!")
                               app.askToken();
                               //var localcode = loadRequest.url.toString().substr(25);
                               //statusLabel.setText("oauth code found, requesting token");
                               //app.initiateRequest(localcode);
                           } 
                           //else lbl.setText(loadRequest.url)
                       }
                       else if (loadRequest.status ==  WebLoadStatus.Failed ) {
                              lbl.setText("Load failed." + loadRequest.url)
                       }
                    }
                }
        }
        
    }
}

