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

}
