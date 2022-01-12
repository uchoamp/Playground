using System.Text.Json.Serialization;

namespace LacunaGenetics.Models;

public class User: JsonObject
{

    [JsonPropertyName("username")]
    public string Username { get; set; }

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
    [JsonPropertyName("email")]
    public string? Email { get; set; }

    [JsonPropertyName("password")]
    public string Password { get; set; }

    public User(string username, string email, string password)
    {
        Username = username;
        Email = email;
        Password = password;
    }

    public User(string username, string password)
    {
        Username = username;
        Password = password;
    }


}
