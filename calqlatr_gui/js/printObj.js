
var printObj = typeof JSON != "undefined" ? JSON.stringify : function(obj) {
    var arr = [];
    $.each(obj, function(key, val) {
        var next = key + ": ";
        next += $.isPlainObject(val) ? printObj(val) : val;
        arr.push( next );
    });
    return "{ " +  arr.join(", ") + " }";
}


function formatJson( jsonStr) {
    var level = 0;
    var jsonForMatStr ="" ;
    for(var i=0;i<jsonStr.length;i++){
        var c = jsonStr.charAt(i);
        if(level>0&&'\n'==jsonForMatStr.charAt(jsonForMatStr.length-1)){
            jsonForMatStr = jsonForMatStr + getLevelStr(level);
        }
        switch (c) {
        case '{':
        case '[':
            jsonForMatStr = jsonForMatStr + c+"\n";
            level++;
            break;
        case ',':
            jsonForMatStr= jsonForMatStr + c+"\n";
            break;
        case '}':
        case ']':
            jsonForMatStr= jsonForMatStr +"\n";
            level--;
            jsonForMatStr= jsonForMatStr +getLevelStr(level);
            jsonForMatStr= jsonForMatStr + c;
            break;
        default:
            jsonForMatStr = jsonForMatStr + c;
            break;
        }
    }

    return jsonForMatStr;

}

function getLevelStr( level){
    var levelStr = "";
    for(var levelI = 0;levelI<level ; levelI++){
        levelStr= levelStr + "  ";
    }
    return levelStr;
}
