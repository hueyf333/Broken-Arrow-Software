# Contributing to Broken Arrow M900

Thank you for your interest in contributing to Broken Arrow M900!

## Development Setup

1. **Fork the repository**
   ```bash
   git clone https://github.com/yourusername/Broken-Arrow-Software.git
   ```

2. **Install JUCE**
   - Download JUCE 7.0.5 or later from https://juce.com/download/
   - Extract to a known location

3. **Configure build environment**
   - Windows: Visual Studio 2022
   - macOS: Xcode 14+
   - Linux: GCC 11+ or Clang 14+

4. **Open project**
   - Launch Projucer
   - Open `Broken Arrow M900.jucer`
   - Set module paths to your JUCE installation
   - Save and generate project files

## Code Style

### C++ Guidelines

- **Standard**: C++20
- **Naming**:
  - Classes: `PascalCase` (e.g., `PreampStage`)
  - Methods: `camelCase` (e.g., `processBlock`)
  - Variables: `camelCase` (e.g., `sampleRate`)
  - Constants: `camelCase` with `constexpr` or `const` (e.g., `gateAttackMs`)
  - Member variables: No prefix (rely on `this->` when needed)

- **Formatting**:
  - Indent: 4 spaces (no tabs)
  - Braces: Opening brace on same line
  - Line length: 120 characters max
  - Use auto judiciously (when type is obvious)

- **Best Practices**:
  - Real-time safe: No allocations on audio thread
  - Use JUCE idioms and types
  - Prefer RAII for resource management
  - Mark overrides with `override` keyword
  - Use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR`

### Example
```cpp
class NewDSPModule
{
public:
    NewDSPModule();
    ~NewDSPModule() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setParameter(float value);

private:
    juce::SmoothedValue<float> parameter{ 0.5f };
    double sampleRate = 44100.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewDSPModule)
};
```

## Making Changes

### Branch Naming

- Features: `feature/description`
- Bugfixes: `bugfix/description`
- Hotfixes: `hotfix/description`
- Examples: `feature/add-ir-browser`, `bugfix/fix-gate-threshold`

### Commit Messages

Use conventional commits:
```
type(scope): description

[optional body]

[optional footer]
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `test`: Test additions/changes
- `chore`: Build/tooling changes

Examples:
```
feat(dsp): add power amp bias control

Add bias parameter to power amp stage for more tonal variety.

Closes #42
```

```
fix(ui): prevent meter clipping indicator stuck

Reset clip indicator after 500ms timeout.
```

## Pull Request Process

1. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature
   ```

2. **Make your changes**
   - Write clean, documented code
   - Add tests if applicable
   - Update documentation

3. **Test thoroughly**
   - Run DSP test harness
   - Test in multiple DAWs
   - Check different sample rates
   - Verify all plugin formats

4. **Commit changes**
   ```bash
   git add .
   git commit -m "feat(scope): description"
   ```

5. **Push to your fork**
   ```bash
   git push origin feature/your-feature
   ```

6. **Create Pull Request**
   - Provide clear description
   - Reference related issues
   - Include testing notes
   - Add screenshots for UI changes

### PR Checklist

- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] New tests added for new features
- [ ] Documentation updated
- [ ] No compiler warnings
- [ ] Changes are backwards compatible (or noted)
- [ ] Tested on target platforms

## Areas for Contribution

### High Priority
- User IR file browser implementation
- Extended preset management UI
- Performance optimizations
- Additional cabinet models
- Documentation improvements

### DSP Enhancements
- More preamp voicings
- Additional tone stack models
- Power tube simulation improvements
- Transformer saturation
- Speaker breakup modeling

### UI Improvements
- Visual spectrum analyzer
- Advanced metering
- A/B comparison
- Skin/theme system
- Accessibility features

### Testing
- Automated DSP tests
- Frequency response validation
- Platform-specific testing
- Performance benchmarking
- Stress testing

## Code Review Process

1. Maintainer reviews code
2. Automated tests run
3. Discussion of changes
4. Revisions if needed
5. Approval and merge

## Getting Help

- **Documentation**: See README.md and TESTING.md
- **Issues**: Open a GitHub issue
- **Discussions**: Use GitHub Discussions
- **Email**: support@brokenarrowsoftware.com

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (see LICENSE file).

## Code of Conduct

### Our Pledge

We pledge to make participation in our project a harassment-free experience for everyone.

### Our Standards

- Be respectful and inclusive
- Accept constructive criticism
- Focus on what's best for the project
- Show empathy towards others

### Unacceptable Behavior

- Harassment or discriminatory language
- Personal attacks
- Publishing others' private information
- Other unethical or unprofessional conduct

## Attribution

Thank you to all contributors who help make Broken Arrow M900 better!
