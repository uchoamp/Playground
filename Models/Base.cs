using System.Text.Json;

namespace LacunaGenetics.Models;


public abstract class JsonObject
{
    public override string ToString()
    {
        return ToJson(true);
    }
   
    public string ToJson(bool writedIndented = false)
    {

        return JsonSerializer.Serialize(this, GetType(), new JsonSerializerOptions { WriteIndented = writedIndented});
    }
}

