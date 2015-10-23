import bb.cascades 1.0
import custom 1.0
Page {
    objectName: "tutorialpage"
    property alias myWebView : myWebView
    Container {
        layout: StackLayout { orientation: LayoutOrientation.TopToBottom }

        ScrollView {
             scrollViewProperties {
                    scrollMode: ScrollMode.Both
                    pinchToZoomEnabled: true
                    maxContentScale: 10
                    minContentScale: 1 
                }
            WebView {
                url: "local:///assets/web/tutorial.html"
                id: myWebView
                objectName: "myWebView"
               
                onMinContentScaleChanged: {
                               scrollView.scrollViewProperties.minContentScale = minContentScale;
                }
                onMaxContentScaleChanged: {
                               scrollView.scrollViewProperties.maxContentScale = maxContentScale;
                }
            }
        }
   
    }  
    
    onCreationCompleted: {  }
    
}
