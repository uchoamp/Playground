using System.Text.Json.Serialization;


namespace LacunaGenetics.Models;

public class Response : JsonObject
{

    [JsonPropertyName("code")]
    public string Code { get; set; }

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
    [JsonPropertyName("message")]
    public string? Message { get; set; }

    public Response()
    {
        Code = "Fail";
    }

}

public class ResponseToken : Response 
{

    [JsonPropertyName("accessToken")]
    public string? AccessToken{ get; set; }

}


