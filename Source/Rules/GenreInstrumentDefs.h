#pragma once

#include <juce_core/juce_core.h>

enum class Genre
{
    Rock = 0,
    Pop,
    HipHop,
    EDM,
    Jazz,
    RnB,
    Metal,
    Classical,
    NumGenres
};

enum class Instrument
{
    Vocals = 0,
    Drums,
    Bass,
    ElectricGuitar,
    AcousticGuitar,
    KeysSynths,
    Strings,
    Brass,
    NumInstruments
};

inline juce::StringArray getGenreNames()
{
    return { "Rock", "Pop", "Hip-Hop", "EDM", "Jazz", "R&B", "Metal", "Classical" };
}

inline juce::StringArray getInstrumentNames()
{
    return { "Vocals", "Drums", "Bass", "Electric Guitar", "Acoustic Guitar",
             "Keys/Synths", "Strings", "Brass" };
}
