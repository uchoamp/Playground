using System.Text;
using System.Text.Json;
using System.Net.Http.Headers;
using LacunaGenetics.Models;




namespace LacunaGenetics;

public class Client : HttpClient
{

    public Client(Uri baseAddress) : base()
    {
        BaseAddress = baseAddress;
        DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
        DefaultRequestHeaders.Add("User-Agent", ".NET Foundation Repository Reporter");
    }

    public Client(string baseAddress) : base()
    {
        BaseAddress = new Uri(baseAddress);
        DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
        DefaultRequestHeaders.Add("User-Agent", ".NET Foundation Repository Reporter");
    }

    public async Task<Response?> CreateUser(User user)
    {

        var requestContent = new StringContent(user.ToJson(), Encoding.UTF8, "application/json");

        var requestResponse = await PostAsync("/api/users/create", requestContent);

        var content = await requestResponse.Content.ReadAsStreamAsync();
        var response = await JsonSerializer.DeserializeAsync<Response>(content);

        return response;
    }


    public async Task Login(User user)
    {

        var requestContent = new StringContent(user.GetJsonCredentials(), Encoding.UTF8, "application/json");

        var requestResponse = await PostAsync("/api/users/login", requestContent);

        var content = await requestResponse.Content.ReadAsStreamAsync();

        var response = await JsonSerializer.DeserializeAsync<ResponseToken>(content);

        if (response != null && response.Code != "Success")
        {
            throw new Exception(response.Message != null ? $"{response.Message} with status code {response.Code}." : $"Status code {response.Code}.");
        }

        DefaultRequestHeaders.Add("Authorization", "Bearer " + response?.AccessToken);
        Console.WriteLine($"User {user.Username} is logged in.\n");
    }

    public async Task<Job?> GetJob()
    {
        var requestResponse = GetStreamAsync("/api/dna/jobs");

        var response = await JsonSerializer.DeserializeAsync<ResponseJob>(await requestResponse);

        return response != null ? response.Job : null;
    }

    public async Task<Response> MakeTheJob(Job job)
    {
        string jobDone = job.GetJsonJobDone();
        var requestContent = new StringContent(jobDone, Encoding.UTF8, "application/json");

        var requestResponse = await PostAsync("/api/dna/jobs/" + job.Id + "/" + job.GetPathDelivery(), requestContent);

        var content = await requestResponse.Content.ReadAsStreamAsync();

        var response = await JsonSerializer.DeserializeAsync<Response>(content);

        if (response == null)
            throw new ArgumentException("Impossible to continue, with null response", nameof(response));

        return response;
    }
}
