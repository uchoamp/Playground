using System.Text;
using System.Text.Json;
using System.Net.Http.Headers;
using LacunaGenetics.Models;



namespace LacunaGenetics;

public class Client: HttpClient
{

    public Client(Uri baseAddress): base()  
    {
        BaseAddress = baseAddress;
        DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
        DefaultRequestHeaders.Add("User-Agent", ".NET Foundation Repository Reporter");
    }

    public Client(string baseAddress): base()  
    {
        BaseAddress = new Uri(baseAddress);
        DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
        DefaultRequestHeaders.Add("User-Agent", ".NET Foundation Repository Reporter");
    }

    public async Task<Response> CreateUser(User user){

        var requestContent = new StringContent(user.ToJson(), Encoding.UTF8, "application/json");

        var requestResponse = await PostAsync("/api/users/create", requestContent);


        var content = await requestResponse.Content.ReadAsStreamAsync();
        var response = await JsonSerializer.DeserializeAsync<Response>(content); 

        return response ?? new Response(); 
    }


}
