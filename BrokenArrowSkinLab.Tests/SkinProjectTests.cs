using Microsoft.VisualStudio.TestTools.UnitTesting;
using BrokenArrowSkinLab.Models;
using System.Linq;

namespace BrokenArrowSkinLab.Tests;

[TestClass]
public class SkinProjectTests
{
    [TestMethod]
    public void SkinProject_Creation_InitializesWithDefaultFrame()
    {
        // Arrange & Act
        var project = new SkinProject();

        // Assert
        Assert.IsNotNull(project.Frames);
        Assert.AreEqual(1, project.Frames.Count);
        Assert.AreEqual(1, project.Frames[0].Count);
        Assert.AreEqual("Background", project.Frames[0][0].Name);
    }

    [TestMethod]
    public void SkinProject_GetCurrentFrameLayers_ReturnsCorrectFrame()
    {
        // Arrange
        var project = new SkinProject();
        project.CurrentFrame = 0;

        // Act
        var layers = project.GetCurrentFrameLayers();

        // Assert
        Assert.IsNotNull(layers);
        Assert.AreEqual(1, layers.Count);
    }

    [TestMethod]
    public void SkinProject_DefaultValues_AreCorrect()
    {
        // Arrange & Act
        var project = new SkinProject();

        // Assert
        Assert.AreEqual("Untitled Project", project.Name);
        Assert.AreEqual(512, project.CanvasWidth);
        Assert.AreEqual(512, project.CanvasHeight);
        Assert.AreEqual(1, project.FrameCount);
        Assert.AreEqual(0, project.CurrentFrame);
        Assert.AreEqual(30, project.FrameRate);
    }

    [TestMethod]
    public void SkinProject_GetCurrentFrameLayers_InvalidFrame_ReturnsEmpty()
    {
        // Arrange
        var project = new SkinProject();
        project.CurrentFrame = 99;

        // Act
        var layers = project.GetCurrentFrameLayers();

        // Assert
        Assert.IsNotNull(layers);
        Assert.AreEqual(0, layers.Count);
    }
}
