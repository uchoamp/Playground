using LacunaGenetics.Models;

namespace LacunaGenetics
{

    class Program
    {

        private static readonly Client client = new Client("https://gene.lacuna.cc ");
        private static readonly User user = new("uchoamp", "marcospacheco10111@gmail.com", "aupaatleti");

        public static async Task Main(string[] args)
        {
            //Create a user 
            Response response = await client.CreateUser(user);
            Console.WriteLine(response);
        }

    }

}
