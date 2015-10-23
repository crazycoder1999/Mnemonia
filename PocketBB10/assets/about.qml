import bb.cascades 1.0

Page {
    Container {
        leftPadding: 20;
        rightPadding: 20;
        topPadding: 30;
         background: Color.create ("#F1E3AD")
        Label {
            text: "Welcome to Mnemonia";
            horizontalAlignment: HorizontalAlignment.Center;
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontWeight: FontWeight.Bold
                fontSize: FontSize.XLarge
                color: Color.create("#FF6C42")    
                
            }
        }
        Divider {}
        Label {
            text: "<html>This is an <b>App</b>, made for users of <a href=\"http://www.getpocket.com\">getPocket.com</a>. It helps to save links and let read them later.</html>";
            horizontalAlignment: HorizontalAlignment.Center;
            multiline: true;
            textStyle { color: Color.create("#000000")}
        }
        Divider {
        }
        Label {
            text: "Follow Mnemonia on Twitter, for news, bugreports or request:"
            horizontalAlignment: HorizontalAlignment.Left
            multiline: true
            textStyle {
                color: Color.create("#000000")
            }
        }
        Label {
            text: "<html><a href=\"https://twitter.com/MnemoniaApp\">@MnemoniaApp</a></html>"
            horizontalAlignment: HorizontalAlignment.Center
        }

        Label {
            text: "Your feedbacks are important!"
            multiline: true
            horizontalAlignment: HorizontalAlignment.Center
            textStyle {
                color: Color.create("#000000")
            }
        }
        Divider {}
        Label {
            text: qsTr("Do you like Mnemonia?")
            horizontalAlignment: HorizontalAlignment.Center
            textFormat: TextFormat.Html
            textStyle {
                color: Color.create("#000000")
            }
        }
        Label {
            text: qsTr("Please <a mimetype=\"application/x-bb-appworld\" href=\"http://appworld.blackberry.com/webstore/content/22914875\">Rate Mnemonia</a>")
            horizontalAlignment: HorizontalAlignment.Center
            textFormat: TextFormat.Html
            textStyle {  color: Color.create("#000000") }
        }
        
        Divider{}
        Label {
            topPadding: 20;
            text: "<html><a href=\"http://www.privacychoice.org/policy/mobile?policy=c8b3b6b8cf901ccc68a7e963251f5298\"><u>Privacy Policy</u></a></html>"
            horizontalAlignment: HorizontalAlignment.Center
        }
    }
}
