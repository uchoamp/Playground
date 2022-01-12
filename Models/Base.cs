using System.Text.Json;

namespace LacunaGenetics.Models;


public abstract class JsonObject
{
    public override string ToString()
    {
        return ToJson(true);
    }
   
    public virtual string ToJson(bool writedIndented = false )
    {

        var serializeOptions = new JsonSerializerOptions { WriteIndented = writedIndented };


        return JsonSerializer.Serialize(this, GetType(), serializeOptions);
    }
}

