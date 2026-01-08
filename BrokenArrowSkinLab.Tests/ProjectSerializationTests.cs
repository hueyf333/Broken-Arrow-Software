using Microsoft.VisualStudio.TestTools.UnitTesting;
using BrokenArrowSkinLab.Models;
using BrokenArrowSkinLab.Services;
using System;
using System.IO;
using System.Threading.Tasks;

namespace BrokenArrowSkinLab.Tests;

[TestClass]
public class ProjectSerializationTests
{
    private ProjectSerializationService? _service;
    private string? _testFilePath;

    [TestInitialize]
    public void Setup()
    {
        _service = new ProjectSerializationService();
        _testFilePath = Path.Combine(Path.GetTempPath(), $"test_{Guid.NewGuid()}.basl");
    }

    [TestCleanup]
    public void Cleanup()
    {
        if (_testFilePath != null && File.Exists(_testFilePath))
        {
            File.Delete(_testFilePath);
        }
    }

    [TestMethod]
    public async Task SaveProject_CreatesFile()
    {
        // Arrange
        var project = new SkinProject
        {
            Name = "Test Project",
            CanvasWidth = 512,
            CanvasHeight = 512
        };

        // Act
        var result = await _service!.SaveProjectAsync(project, _testFilePath!);

        // Assert
        Assert.IsTrue(result);
        Assert.IsTrue(File.Exists(_testFilePath));
    }

    [TestMethod]
    public async Task SaveAndLoadProject_PreservesData()
    {
        // Arrange
        var originalProject = new SkinProject
        {
            Name = "Test Project",
            CanvasWidth = 1024,
            CanvasHeight = 768,
            FrameCount = 1
        };
        
        originalProject.Frames[0].Add(new Layer
        {
            Name = "Test Layer",
            Position = new System.Numerics.Vector2(100, 200)
        });

        // Act
        await _service!.SaveProjectAsync(originalProject, _testFilePath!);
        var loadedProject = await _service.LoadProjectAsync(_testFilePath!);

        // Assert
        Assert.IsNotNull(loadedProject);
        Assert.AreEqual(originalProject.Name, loadedProject.Name);
        Assert.AreEqual(originalProject.CanvasWidth, loadedProject.CanvasWidth);
        Assert.AreEqual(originalProject.CanvasHeight, loadedProject.CanvasHeight);
        Assert.AreEqual(1, loadedProject.Frames.Count);
        Assert.AreEqual(1, loadedProject.Frames[0].Count);
        Assert.AreEqual("Test Layer", loadedProject.Frames[0][0].Name);
    }

    [TestMethod]
    public async Task LoadProject_NonExistentFile_ReturnsNull()
    {
        // Act
        var result = await _service!.LoadProjectAsync("nonexistent.basl");

        // Assert
        Assert.IsNull(result);
    }

    [TestMethod]
    public void GetFileFilter_ReturnsCorrectFilter()
    {
        // Act
        var filter = _service!.GetFileFilter();

        // Assert
        Assert.IsTrue(filter.Contains(".basl"));
    }
}
