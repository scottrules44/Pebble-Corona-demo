local pebble = require "plugin.pebble"

local sendBox = native.newTextField( display.contentCenterX, display.contentCenterY-100, 150, 20 )

local myText = display.newText( "send", display.contentCenterX, display.contentCenterY-60 , native.systemFont )
myText.alpha = 1
myText:addEventListener( "tap", function ( e )
    pebble.send(sendBox.text, function(ev)
        local json = require("json")
            print("send")
            print("--------")
            print(json.encode(ev))
            print("--------")
    end)
end )

local myText2 = display.newText( "Message:", display.contentCenterX, display.contentCenterY-30 , native.systemFont )

local myText3 = display.newText( "Not connected", display.contentCenterX, display.contentCenterY , native.systemFont )

pebble.init("16ac75b9-e96f-486e-96e0-492efc1af7f0", function(e)
    local json = require("json")
    if(e) then
        print("init")
        print("--------")
        print(json.encode(e))
        print("--------")
    end
    if (e.type == "watch connection") then
        myText3.text = e.response
    end
    if (e.type == "app message") then
        myText2.text = "Message:"..e.response
    end
end, true)

