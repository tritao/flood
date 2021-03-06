
project "EngineWeaver.Tests"

    kind "SharedLib"
    language "C#"
    
    location "."
    
    files
    {
        "EngineWeaver.Tests.lua",
        "**.cs",
    }
    
    links
    {
        "System",
        "EngineManaged",
		"EngineWeaver",
        "Mono.Cecil",
		path.join(depsdir,"NUnit","nunit.framework")
    }