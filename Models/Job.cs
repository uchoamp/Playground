using System.Text.Json.Serialization;

namespace LacunaGenetics.Models;

public class Job: JsonObject
{

    [JsonPropertyName("id")]
    public int Id { get; set; }

}
