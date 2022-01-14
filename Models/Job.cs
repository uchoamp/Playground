using System.Text.Json.Serialization;
using System.Reflection;
using System.Text;

namespace LacunaGenetics.Models;

public enum ReturnType
{
    Void,
    String,
    Bool,
}

public class RequestJob : JsonObject
{

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
    [JsonPropertyName("strand")]
    public string? Strand { get; set; }

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
    [JsonPropertyName("strandEncoded")]
    public string? StrandEncoded { get; set; }

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
    [JsonPropertyName("isActivated")]
    public bool? IsActivated { get; set; }

    public static RequestJob GetRequestWithStrand(string strand)
    {
        return new RequestJob { Strand = strand };
    }

    public static RequestJob GetRequestWithStrandEncoded(string strandEncoded)
    {
        return new RequestJob { StrandEncoded = strandEncoded };
    }

    public static RequestJob GetRequestWithIsActivated(bool isActivated)
    {
        return new RequestJob { IsActivated = isActivated };
    }
}

public class JobMeta
{
    public readonly string Description;
    public readonly string PathUri;
    public readonly ReturnType FuncReturnType;

    public JobMeta(string description, string pathUri, ReturnType funcReturnType)
    {
        Description = description;
        PathUri = pathUri;
        FuncReturnType = funcReturnType;
    }
}

public class JobHandler<TResult> : JobMeta
{
    public readonly Func<Job, TResult> MakeJob;
    public readonly Func<TResult, RequestJob> GetRequestJob;


    public JobHandler(string description, string pathUri, Func<Job, TResult> makeJob, Func<TResult, RequestJob> getRequestJob, ReturnType funcReturnType) : base(description, pathUri, funcReturnType)
    {
        MakeJob = makeJob;
        GetRequestJob = getRequestJob;
    }
}



public class Job : JsonObject
{

    public static readonly Dictionary<string, JobMeta> JobsType = new Dictionary<string, JobMeta>
    {
        ["DecodeStrand"] = new JobHandler<string>("Decode the strand.", "decode", GetMakeJob<string>("DecodeStrand"), RequestJob.GetRequestWithStrand, ReturnType.String),
        ["EncodeStrand"] = new JobHandler<string>("Encode the strand.", "encode", GetMakeJob<string>("EncodeStrand"), RequestJob.GetRequestWithStrandEncoded, ReturnType.String),
        ["CheckGene"] = new JobHandler<bool>("Check if the gene is activated.", "gene", GetMakeJob<bool>("CheckGene"), RequestJob.GetRequestWithIsActivated, ReturnType.Bool),
    };

    public static readonly Dictionary<string, string> StringToBinaryReference = new Dictionary<string, string>
    {
        ["A"] = "00",
        ["C"] = "01",
        ["G"] = "10",
        ["T"] = "11",
        ["00"] = "A",
        ["01"] = "C",
        ["10"] = "G",
        ["11"] = "T",
    };

    public static readonly Dictionary<char, char> Pair = new Dictionary<char, char>
    {
        ['A'] = 'X',
        ['C'] = 'Y',
        ['G'] = 'C',
        ['T'] = 'A',
        ['X'] = 'T',
        ['Y'] = 'G',
    };

    [JsonPropertyName("id")]
    public string? Id { get; set; }

    [JsonPropertyName("type")]
    public string? Type { get; set; }

    [JsonPropertyName("strand")]
    public string? Strand { get; set; }

    [JsonPropertyName("strandEncoded")]
    public string? StrandEncoded { get; set; }

    [JsonPropertyName("geneEncoded")]
    public string? GeneEncoded { get; set; }

    public string GetJsonJobDone()
    {
        if (Type == null)
            throw new ArgumentException("The atribute canot be null.", nameof(Type));
        RequestJob requestJob = new();

        var jobMeta = (JobMeta)JobsType[Type];
        if (jobMeta != null)
        {
            if (jobMeta.FuncReturnType == ReturnType.String)
            {
                var jobHandler = (JobHandler<string>)jobMeta;
                string result = jobHandler.MakeJob(this);
                requestJob = jobHandler.GetRequestJob(result);

            }
            else
            {
                var jobHandler = (JobHandler<bool>)jobMeta;
                bool result = jobHandler.MakeJob(this);
                requestJob = jobHandler.GetRequestJob(result);
            }
        }
        return requestJob.ToJson();
    }



    private static Func<Job, T> GetMakeJob<T>(string methodName)
    {

        return (Job @this) =>
        {
            MethodInfo? makeJob = @this.GetType().GetMethod(methodName, BindingFlags.Instance | BindingFlags.Public);

            if (makeJob == null)
                throw new Exception($"Not found a method to make the job {methodName}");

            return (T)(makeJob.Invoke(@this, null) ??
                throw new Exception($"Make job is returned a null reference."));
        };

    }

    public string DecodeStrand()
    {
        if (StrandEncoded == null)
            throw new ArgumentException("The atribute canot be null.", nameof(StrandEncoded));

        return DecodeStrand(StrandEncoded);
    }

    public string EncodeStrand()
    {
        if (Strand == null)
            throw new ArgumentException("The atribute canot be null.", nameof(Strand));

        return EncodeStrand(Strand);
    }

    public bool CheckGene()
    {
        if (StrandEncoded == null)
            throw new ArgumentException("The atribute canot be null.", nameof(StrandEncoded));
        if (GeneEncoded == null)
            throw new ArgumentException("The atribute canot be null.", nameof(GeneEncoded));

        StringBuilder templateDecoded = new StringBuilder(DecodeStrand(StrandEncoded));
        string geneDecoded = DecodeStrand(GeneEncoded);

        if (templateDecoded[0] == 'G')
            foreach (var nb in Pair)
                templateDecoded.Replace(nb.Key, nb.Value);


        return CheckGene(geneDecoded, templateDecoded.ToString());
    }

    public static string DecodeStrand(string strandEncoded)
    {
        return StrandByteArrayToString(Convert.FromBase64String(strandEncoded));
    }

    public static string EncodeStrand(string strand)
    {
        return Convert.ToBase64String(StrandStringToByteArray(strand));
    }

    public static bool CheckGene(string geneDecoded, string templateDecoded)
    {

        int minStrandSize = geneDecoded.Length / 2;

        for (int i = 0; i < minStrandSize; i++)
        {
            if (templateDecoded.Contains(geneDecoded.Substring(i, minStrandSize)))
                return true;
        }

        return false;
    }

    public static byte[] StrandStringToByteArray(string strStrand)
    {
        string strByteStrand;

        if (strStrand.Length % 4 != 0) throw new ArgumentException("Invalid parameter, format of strand is incorrect.", nameof(strStrand));


        byte[] bytesStrand = new byte[strStrand.Length / 4];

        for (int i = 0; i < bytesStrand.Length; i++)
        {
            strByteStrand = String.Join("", from s in strStrand.Substring(0, 4) select StringToBinaryReference[Convert.ToString(s)]);
            bytesStrand[i] = Convert.ToByte(strByteStrand, 2);
            strStrand = strStrand.Substring(4);
        }

        return bytesStrand;
    }

    public static string StrandByteArrayToString(byte[] byteArrayStrand)
    {
        int i;

        StringBuilder buildStrStrand = new StringBuilder(byteArrayStrand.Length * 4);
        string strByte;
        foreach (var b in byteArrayStrand)
        {
            strByte = Convert.ToString(b, 2);

            if (strByte.Length < 8)
            {
                strByte = new string('0', 8 - strByte.Length) + strByte;
            }

            for (i = 0; i < strByte.Length; i += 2)
            {
                buildStrStrand.Append(StringToBinaryReference[strByte.Substring(i, 2)]);
            }
        }

        return buildStrStrand.ToString();
    }


    public string GetPathDelivery()
    {
        if (Type == null)
            throw new ArgumentException("The atribute canot be null.", nameof(Type));
        return JobsType[Type].PathUri;
    }

    public string GetJobDescription()
    {
        if (Type == null)
            throw new ArgumentException("The atribute canot be null.", nameof(Type));
        return JobsType[Type].Description;
    }
}
