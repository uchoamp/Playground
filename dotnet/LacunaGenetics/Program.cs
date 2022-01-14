using LacunaGenetics.Models;

namespace LacunaGenetics
{

    class Program
    {

        private static readonly Client client = new Client("https://gene.lacuna.cc ");
        private static readonly User user = new("uchoamp", "marcospacheco10111@gmail.com", "aupaatleti");

        public static async Task Main(string[] args)
        {
            Console.CancelKeyPress += (sender, args) =>
            {
                Console.WriteLine("\nEnd of Work!!!");

                Environment.Exit(0);
            };
            //Response? response = await client.CreateUser(user);
            //Console.WriteLine(response);

            Job? job;
            Response jobResult;

            await client.Login(user);
            while (true)
            {
                Console.WriteLine("Press Ctrl - C to STOP working.");
                job = await client.GetJob();

                if (job != null)
                {

                    Console.WriteLine("Making the job: " + job.GetJobDescription());
                    jobResult = await client.MakeTheJob(job);

                    Console.WriteLine("Result of work: " + (jobResult.Message != null ? jobResult.Message : jobResult.Code) + "\n");

                    if (jobResult.Code == "Fail")
                    {
                        Console.WriteLine(job);
                        throw new Exception("Job is failed.");
                    }

                }
                else
                {
                    Console.WriteLine("Work is over, time to rest.");
                    break;
                }
                Thread.Sleep(5000);
            }
        }

    }

}
